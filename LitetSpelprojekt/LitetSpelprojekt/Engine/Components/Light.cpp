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
	position(0, 0, 0),
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
		D3D11_BIND_SHADER_RESOURCE
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
		Vector3 dirLightDirection = Vector3(1, -1, 1);
		dirLightDirection.Normalize();
		this->dirLightProps.direction = dirLightDirection;

		this->position = Vector3(0,0,0);

		// Matrices
		this->viewMatrix = Matrix::CreateLookAt(
			this->position,
			this->position + this->dirLightProps.direction,
			Vector3(0, 1, 0)
		);
		this->projectionMatrix = Matrix::CreateOrthographic(
			100,
			100,
			0.1f,
			100.0f
		);

		break;
	}
}

void Light::render(Scene& scene)
{
	Matrix vpMatrix = this->viewMatrix * this->projectionMatrix;

	std::vector<MeshComp*> meshComponents = scene.getActiveComponents<MeshComp>();

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
		&this->renderer->getMatrixConstantBuffer().getBuffer()
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
		Mesh& mesh = this->resources->getMesh(meshComponents[i]->getMeshName().c_str());
		unsigned int numIndices = mesh.getEntireSubmesh().numIndices;

		// Set mvp Matrix
		Matrix m = meshComponents[i]->getTransform()->getWorldMatrix() * vpMatrix;
		this->renderer->getMatrixBufferStruct().mvpMat = m.Transpose();
		this->renderer->getMatrixConstantBuffer().updateBuffer(&this->renderer->getMatrixBufferStruct());

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
}
