#include "Renderer.h"
#include "../Dev/Log.h"

using namespace DirectX::SimpleMath;

bool Renderer::createInterfaces()
{
	UINT flags = 0;
#ifdef _DEBUG
	flags = D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_0 };

	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};

	swapChainDesc.BufferDesc.Width = this->window->getWidth();
	swapChainDesc.BufferDesc.Height = this->window->getHeight();
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	swapChainDesc.BufferUsage = DXGI_USAGE_UNORDERED_ACCESS | DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 1;
	swapChainDesc.OutputWindow = this->window->getWindowHandle();
	swapChainDesc.Windowed = true;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = 0;

	return !(FAILED(D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, flags, featureLevels, 1, D3D11_SDK_VERSION,
		&swapChainDesc, &this->swapChain, &this->device, nullptr, &this->immediateContext)));
}

bool Renderer::createViews()
{
	// Get address of the back buffer
	ID3D11Texture2D* backBuffer = nullptr;
	if (FAILED(swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer))))
	{
		Log::error("Backbuffer Texture not found.");
		return false;
	}

	// Get buffer description
	D3D11_TEXTURE2D_DESC backBufferDesc{};
	backBuffer->GetDesc(&backBufferDesc);

	// Create back buffer UAV
	this->backBufferUAV.createTextureUAV(backBuffer, backBufferDesc.Format);

	if (FAILED(device->CreateRenderTargetView(backBuffer, NULL, &this->backBufferRTV)))
	{
		Log::error("Failed to create backbufferRTV");
	}
	backBuffer->Release();

	// Depth/stencil texture
	this->dsTexture.createAsDepthTexture(
		this->window->getWidth(), this->window->getHeight(),
		DXGI_FORMAT_R32_TYPELESS
	);

	// Depth/stencil view
	this->dsView.createDSV(
		*this, this->dsTexture.getPtr(), DXGI_FORMAT_D32_FLOAT
	);

	//Depth stencil state
	D3D11_DEPTH_STENCIL_DESC dssDesc{};
	ZeroMemory(&dssDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	dssDesc.DepthEnable = true;
	dssDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dssDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

	if (FAILED(device->CreateDepthStencilState(&dssDesc, &this->dsState)))
	{
		Log::error("Failed to create Depth Stencil State");
		return false;
	}

	immediateContext->OMSetDepthStencilState(this->dsState, 0);
	
	return true;
}

bool Renderer::loadShaders()
{
	// Input layout desc
	InputLayoutDesc inputLayoutDesc;
	inputLayoutDesc.add("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	inputLayoutDesc.add("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT);
	inputLayoutDesc.add("UV", DXGI_FORMAT_R32G32_FLOAT);

	// Vertex shader
	this->vertexShader.loadVS("Default_VS", inputLayoutDesc);

	// Pixel shader
	this->pixelShader.loadPS("Default_PS");

	return true;
}

Renderer::Renderer(Resources& resources)
	: device(nullptr), immediateContext(nullptr), swapChain(nullptr),
	viewport(), backBufferRTV(nullptr), dsTexture(*this),
	dsView("rendererDSV"),

	vertexShader(*this),
	pixelShader(*this),

	cameraConstantBuffer(*this, "cameraConstantBuffer"),
	compactCameraConstantBuffer(*this, "compactCameraConstantBuffer"),
	backBufferUAV(*this, "backBufferUAV"),

	resources(resources),

	skybox(*this),
	particles(*this)

	//activeCamera(nullptr)
{
}

Renderer::~Renderer()
{
	this->device->Release();
	this->immediateContext->Release();
	this->swapChain->Release();

	this->backBufferRTV->Release();
	this->dsState->Release();
}

void Renderer::init(Window& window)
{
	this->window = &window;

	createInterfaces();
	createViews();
	loadShaders();

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = static_cast<float>(this->window->getWidth());
	viewport.Height = static_cast<float>(this->window->getHeight());
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1;

	// Constant buffers
	this->cameraConstantBuffer.createBuffer(sizeof(CameraBufferData));
	this->compactCameraConstantBuffer.createBuffer(sizeof(CompactCameraBufferData));
	
	//Init skybox
	this->skybox.initialize();
	this->particles.init();

	// Topology won't change during runtime
	immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Renderer::render(Scene& scene)
{
#ifdef _DEBUG
	if (!scene.getActiveCamera())
		Log::error("No active camera has been set in the renderer.");

	unsigned int numDrawCalls = 0;
#endif

	// Clear buffers
	float clearColour[4] = { 0, 0, 0, 0 };
	immediateContext->ClearRenderTargetView(this->backBufferRTV, clearColour);
	immediateContext->ClearDepthStencilView(this->dsView.getPtr(), D3D11_CLEAR_DEPTH, 1.0f, 0);

	// Update camera constant buffer
	Matrix vp;
	vp = scene.getActiveCamera()->getViewMatrix();
	vp *= scene.getActiveCamera()->getProjectionMatrix();
	this->cameraBufferStruct.vpMat = vp.Transpose();

	std::vector<Light*> lightComponents = scene.getActiveComponents<Light>();
	
	// ----- Render shadow maps -----
	for (unsigned int i = 0; i < lightComponents.size(); ++i)
	{
		lightComponents[i]->render(scene);
	}
	Light* firstLight = lightComponents.size() > 0?
		lightComponents[0] : nullptr;

	// ----- Render meshes to back buffer -----
	immediateContext->RSSetViewports(1, &this->viewport);
	immediateContext->VSSetShader(this->vertexShader.getVS(), nullptr, 0);
	immediateContext->PSSetShader(this->pixelShader.getPS(), nullptr, 0);
	immediateContext->OMSetRenderTargets(1, &this->backBufferRTV, this->dsView.getPtr());

	std::vector<MeshComp*> meshComponents = scene.getActiveComponents<MeshComp>();

	immediateContext->VSSetConstantBuffers(0, 1, &this->cameraConstantBuffer.getBuffer());
	if (firstLight != nullptr)
	{
		immediateContext->PSSetConstantBuffers(0, 1, &firstLight->getLightBuffer().getBuffer());
		immediateContext->PSSetConstantBuffers(1, 1, &firstLight->getDirLightBuffer().getBuffer());

		// Set shadow map
		immediateContext->PSSetSamplers(
			1, 1, &firstLight->getShadowMapTexture().getSampler()
		);
		immediateContext->PSSetShaderResources(
			1, 1, &firstLight->getShadowMapTexture().getSRV().getPtr()
		);
	}

	// Render all meshes
	for (unsigned int i = 0; i < meshComponents.size(); ++i)
	{
		Mesh& mesh = this->resources.getMesh(meshComponents[i]->getMeshName().c_str());

		// Set mvp Matrix
		Matrix m = meshComponents[i]->getTransform()->getWorldMatrix();
		this->cameraBufferStruct.modelMat = m.Transpose();
		this->cameraConstantBuffer.updateBuffer(&this->cameraBufferStruct);

		// Set texture
		for (unsigned int j = 0; j < mesh.getSubmeshes().size(); ++j)
		{
			Submesh& currentSubmesh = mesh.getSubmeshes()[j];

			Material& material = this->resources.getMaterial(
				strcmp(currentSubmesh.materialName, "") ?
				currentSubmesh.materialName :
				meshComponents[i]->getMaterialName().c_str()
			);
			Texture& texture = this->resources.getTexture(material.getDiffuseTextureName().c_str());
			immediateContext->PSSetSamplers(
				0, 1, &texture.getSampler()
			);
			immediateContext->PSSetShaderResources(
				0, 1, &texture.getSRV().getPtr()
			);

			// Vertex/index buffer
			immediateContext->IASetInputLayout(this->vertexShader.getInputLayout());
			immediateContext->IASetVertexBuffers(
				0, 1, &mesh.getVertexBuffer().getBuffer(), &mesh.getVertexBuffer().getStride(), &mesh.getVertexBuffer().getOffset());
			immediateContext->IASetIndexBuffer(
				mesh.getIndexBuffer().getBuffer(), DXGI_FORMAT_R32_UINT, 0
			);

			/*immediateContext->DrawIndexed(
				mesh.getIndexBuffer().getIndexCount(), 0, 0
			);*/

			immediateContext->DrawIndexed(
				currentSubmesh.numIndices, currentSubmesh.startIndex, 0
			);

#ifdef _DEBUG
			numDrawCalls++;
#endif
		}

		// Remove shadow map from slot 1
		ID3D11ShaderResourceView* nullSRV[]{ nullptr };
		immediateContext->PSSetShaderResources(
			1, 1, nullSRV
		);
	}

	//Skybox
	immediateContext->IASetInputLayout(this->skybox.getVertexShader().getInputLayout());
	immediateContext->VSSetConstantBuffers(0, 1, &this->skybox.getConstantBuffer().getBuffer());

	// Update mvp Matrix
	this->skybox.update(scene.getActiveCamera()->getTransform()->getPosition(), vp);
	
	//Bind skybox
	immediateContext->IASetVertexBuffers(
		0, 1,
		&this->skybox.getMesh().getVertexBuffer().getBuffer(), 
		&this->skybox.getMesh().getVertexBuffer().getStride(),
		&this->skybox.getMesh().getVertexBuffer().getOffset()
	);

	immediateContext->IASetIndexBuffer(
		this->skybox.getMesh().getIndexBuffer().getBuffer(),
		DXGI_FORMAT_R32_UINT, 0
	);

	CubeMap& cubeMap = resources.getCubemap(this->skyboxName.c_str());

	immediateContext->PSSetSamplers(
		0, 1, &cubeMap.getTexture().getSampler()
	);

	immediateContext->PSSetShaderResources(
		0, 1, &cubeMap.getTexture().getSRV().getPtr()
	);

	immediateContext->VSSetShader(this->skybox.getVertexShader().getVS(), nullptr, 0);
	immediateContext->PSSetShader(this->skybox.getPixelShader().getPS(), nullptr, 0);

	immediateContext->DrawIndexed(
		this->skybox.getMesh().getIndexBuffer().getIndexCount(), 0, 0
	);

	//Particles
	this->particles.render(vp, scene.getActiveCamera()->getTransform()->getRotation());
	
	// Unbind render target
	ID3D11RenderTargetView* nullRTV[1] = { nullptr };
	immediateContext->OMSetRenderTargets(1, nullRTV, nullptr);

#ifdef _DEBUG
	//Log::write("Num draw calls: " + std::to_string(numDrawCalls));
#endif
}

void Renderer::presentSC()
{
	this->swapChain->Present(1, 0);
}

void Renderer::setSkyBoxName(const std::string& name)
{
	this->skyboxName = name;
}
