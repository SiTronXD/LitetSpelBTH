#include "Light.h"
#include "../GameObject.h"
#include "../Resources.h"
#include "../Graphics/Renderer.h"
#include "../Application/Scene.h"

#include <Windows.h>
#include <SimpleMath.h>

using namespace DirectX::SimpleMath;

Light::Light(GameObject& object)
	: Component(object), 
	type(LightType::DIRECTIONAL),
	shadowMapDepthTexture(nullptr),
	shadowMapDSV("shadowMapDSV"),
	shadowMapVS(nullptr),
	resources(nullptr),
	renderer(nullptr)
{
}

Light::~Light()
{
	delete this->shadowMapDepthTexture;
	delete this->lightBuffer;
	delete this->directionalLightBuffer;
}

void Light::init(Resources& resources, Renderer& renderer)
{
	this->resources = &resources;
	this->renderer = &renderer;

	// Create depth texture
	delete this->shadowMapDepthTexture;
	this->shadowMapDepthTexture = new Texture(renderer);
	this->shadowMapDepthTexture->createAsDepthTexture(
		this->SHADOW_MAP_SIZE, this->SHADOW_MAP_SIZE,
		DXGI_FORMAT_R32_TYPELESS,
		D3D11_BIND_SHADER_RESOURCE,
		true
	);

	// Create DSV
	this->shadowMapDSV.createDSV(
		renderer, this->shadowMapDepthTexture->getPtr(), DXGI_FORMAT_D32_FLOAT
	);

	// Input layout desc
	InputLayoutDesc inputLayoutDesc;
	inputLayoutDesc.add("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	inputLayoutDesc.add("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT);
	inputLayoutDesc.add("UV", DXGI_FORMAT_R32G32_FLOAT);
	this->resources->addVertexShader("ShadowMap_VS", inputLayoutDesc);
	this->shadowMapVS = &this->resources->getVertexShader("ShadowMap_VS");

	// Constant buffers
	delete this->lightBuffer;
	this->lightBuffer = new ConstantBuffer(renderer, "lightConstantBuffer");
	this->lightBuffer->createBuffer(sizeof(LightBufferData));

	delete this->directionalLightBuffer;
	this->directionalLightBuffer = new ConstantBuffer(renderer, "directionalLightConstantBuffer");
	this->directionalLightBuffer->createBuffer(sizeof(DirectionalLightBufferData));

	switch (this->type)
	{
	case LightType::DIRECTIONAL:

		// Viewport
		this->shadowMapViewport.TopLeftX = 0;
		this->shadowMapViewport.TopLeftY = 0;
		this->shadowMapViewport.Width = (FLOAT) this->SHADOW_MAP_SIZE;
		this->shadowMapViewport.Height = (FLOAT) this->SHADOW_MAP_SIZE;
		this->shadowMapViewport.MinDepth = 0;
		this->shadowMapViewport.MaxDepth = 1;

		// Directional light direction
		this->updateDirection(Vector3(1, -1, 1));

		// Constant projection matrix
		this->projectionMatrix = Matrix::CreateOrthographic(
			100,
			100,
			0.1f,
			200.0f
		);

		break;
	}

	// Global color
	this->updateColor(Vector3(1.0f, 1.0f, 1.0f));
}

void Light::render(Scene& scene)
{
	// Make position follow camera
	Vector3 position = 
		scene.getActiveCamera()->getTransform()->getPosition() -
		this->dirLightBufferStruct.direction * 50.0f;

	// Try to make the transition a bit "easier"?
	//position.x = std::roundf(position.x * 0.5f) / 0.5f;
	//position.y = std::roundf(position.y * 0.5f) / 0.5f;
	//position.z = std::roundf(position.z * 0.5f) / 0.5f;

	// View matrix
	this->viewMatrix = Matrix::CreateLookAt(
		position,
		position + this->dirLightBufferStruct.direction,
		Vector3(0, 1, 0)
	);
	Matrix vpMatrix = this->viewMatrix * this->projectionMatrix;

	// Find meshes
	std::vector<MeshComp*> meshComponents = scene.getActiveComponents<MeshComp>();
	std::vector<AbsoluteMeshComp*> absoluteMeshComponents = scene.getActiveComponents<AbsoluteMeshComp>();
	meshComponents.insert(
		meshComponents.end(),
		absoluteMeshComponents.begin(),
		absoluteMeshComponents.end()
	);

	ID3D11DeviceContext* immediateContext =
		this->renderer->getDeviceContext();

	// Clear depth texture
	this->shadowMapDSV.clear();

	// Viewport
	immediateContext->RSSetViewports(1, &this->shadowMapViewport);

	// Vertex shader + input layout
	immediateContext->VSSetShader(this->shadowMapVS->getVS(), nullptr, 0);
	immediateContext->IASetInputLayout(this->shadowMapVS->getInputLayout());

	// Set nullptr RTV/pixel shader and correctDSV
	ID3D11RenderTargetView* nullRTV = nullptr;
	immediateContext->OMSetRenderTargets(
		1, 
		&nullRTV, 
		this->shadowMapDSV.getPtr()
	);
	immediateContext->PSSetShader(nullptr, nullptr, 0);

	// Set matrix buffer
	immediateContext->VSSetConstantBuffers(
		0, 
		1, 
		&this->renderer->getCompactCameraConstantBuffer().getBuffer()
	);

	// Null sampler and texture
	ID3D11SamplerState* nullSampler[]{ nullptr };
	ID3D11ShaderResourceView* nullSRV[]{ nullptr };
	immediateContext->PSSetSamplers(
		0, 1, nullSampler
	);
	immediateContext->PSSetShaderResources(
		0, 1, nullSRV
	);

	for (unsigned int i = 0; i < meshComponents.size(); ++i)
	{
		MeshComp* currentMeshComp = meshComponents[i];

		// This mesh doesn't cast a shadow
		if (!currentMeshComp->getCastShadow())
			continue;

		Mesh& mesh = this->resources->getMesh(currentMeshComp->getMeshName().c_str());
		unsigned int numIndices = mesh.getEntireSubmesh().numIndices;

		// Set mvp Matrix
		Matrix m = currentMeshComp->getTransform()->getWorldMatrix() * vpMatrix;
		this->renderer->getCompactCameraBufferStruct().mvpMat = m.Transpose();
		this->renderer->getCompactCameraConstantBuffer().updateBuffer(&this->renderer->getCompactCameraBufferStruct());

		// Vertex/index buffer
		immediateContext->IASetVertexBuffers(
			0, 1, &mesh.getVertexBuffer().getBuffer(), &mesh.getVertexBuffer().getStride(), &mesh.getVertexBuffer().getOffset());
		immediateContext->IASetIndexBuffer(
			mesh.getIndexBuffer().getBuffer(), DXGI_FORMAT_R32_UINT, 0
		);

		// Draw entire mesh
		immediateContext->DrawIndexed(
			numIndices, 0, 0
		);
	}

	// Update constant buffer
	this->lightBufferStruct.vpMatrix = vpMatrix.Transpose();
	this->lightBuffer->updateBuffer(&this->lightBufferStruct);
}

void Light::updateColor(const DirectX::SimpleMath::Vector3& newCol)
{
	// Update color
	this->dirLightBufferStruct.globalColor = newCol;

	// Update constant buffer
	this->directionalLightBuffer->updateBuffer(&this->dirLightBufferStruct);
}

void Light::updateDirection(Vector3 newDir)
{
	// Update dir
	newDir.Normalize();
	this->dirLightBufferStruct.direction = newDir;

	// Update constant buffer
	this->directionalLightBuffer->updateBuffer(&this->dirLightBufferStruct);
}
