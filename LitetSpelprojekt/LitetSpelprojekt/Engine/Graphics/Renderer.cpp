#include "Renderer.h"
#include "../Time.h"
#include "../Dev/Log.h"
#include "../Dev/Helpers.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

bool Renderer::evaluateAdapterModes()
{
	HRESULT result;

	// Create a graphics interface factory
	IDXGIFactory* factory = nullptr;
	result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	if (FAILED(result))
	{
		Log::error("Failed to create DXGI factory.");

		return false;
	}

	// Create an adapter
	IDXGIAdapter* adapter = nullptr;
	result = factory->EnumAdapters(0, &adapter);
	if (FAILED(result))
	{
		Log::error("Failed to create adapter.");

		return false;
	}

	// Get adapter output
	IDXGIOutput* adapterOutput = nullptr;
	result = adapter->EnumOutputs(0, &adapterOutput);
	if (FAILED(result))
	{
		Log::error("Failed to create adapter output.");

		return false;
	}

	// Get appropriate number of modes
	unsigned int numModes = 0;
	result = adapterOutput->GetDisplayModeList(
		DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_ENUM_MODES_INTERLACED,
		&numModes, NULL
	);
	if (FAILED(result))
	{
		Log::error("Failed to get display modes");

		return false;
	}

	// Array of display modes
	DXGI_MODE_DESC* displayModeList = new DXGI_MODE_DESC[numModes];
	result = adapterOutput->GetDisplayModeList(
		DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_ENUM_MODES_INTERLACED,
		&numModes, displayModeList
	);

	// Add resolutions to vector
	UINT lastWidth = 0;
	UINT lastHeight = 0;
	for (unsigned int i = 0; i < numModes; ++i)
	{
		if (displayModeList[i].Width != lastWidth ||
			displayModeList[i].Height != lastHeight)
		{
			// Updaste last resolution
			lastWidth = displayModeList[i].Width;
			lastHeight = displayModeList[i].Height;

			Log::write("res: " + std::to_string(displayModeList[i].Width) +
				" x " + std::to_string(displayModeList[i].Height) +
				" - " + std::to_string(
					(float)displayModeList[i].RefreshRate.Numerator / displayModeList[i].RefreshRate.Denominator));

			// Filter away smaller aspect ratios
			if ((float) displayModeList[i].Width / displayModeList[i].Height < 16.0f / 9.0f - 0.001f)
				continue;

			// Add resolution
			this->supportedResolutions.push_back(
				XMFLOAT2(
					displayModeList[i].Width, 
					displayModeList[i].Height
				)
			);
		}
	}


	delete[] displayModeList;
	displayModeList = nullptr;

	// Deallocate old objects
	S_RELEASE(adapterOutput);
	S_RELEASE(adapter);
	S_RELEASE(factory);

	return true;
}

bool Renderer::createInterfaces()
{
	// Device, device context and swapchain creation
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
	swapChainDesc.Windowed = !this->window->getIsFullscreen();
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
		DXGI_FORMAT_R32_TYPELESS,
		D3D11_BIND_SHADER_RESOURCE
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

	this->outlineComputeShader.init("Outlines_COMP", this->window->getWidth() / 32, this->window->getHeight() / 32, 1);

	return true;
}

void Renderer::renderMesh(MeshComp& meshComp)
{
	Mesh& mesh = this->resources.getMesh(meshComp.getMeshName().c_str());

	// Set pixel shader for this mesh
	PixelShader& meshPixelShader = this->resources.getPixelShader(meshComp.getPixelShaderName().c_str());
	immediateContext->PSSetShader(meshPixelShader.getPS(), nullptr, 0);

	// Set mvp Matrix
	Matrix m = meshComp.getTransform()->getWorldMatrix();
	this->cameraBufferStruct.modelMat = m.Transpose();
	this->cameraConstantBuffer.updateBuffer(&this->cameraBufferStruct);

	// Vertex/index buffer
	immediateContext->IASetInputLayout(this->vertexShader.getInputLayout());
	immediateContext->IASetVertexBuffers(
		0, 1, &mesh.getVertexBuffer().getBuffer(), &mesh.getVertexBuffer().getStride(), &mesh.getVertexBuffer().getOffset());
	immediateContext->IASetIndexBuffer(
		mesh.getIndexBuffer().getBuffer(), DXGI_FORMAT_R32_UINT, 0
	);

	// Pixel shader constant buffer
	this->pixelShaderBufferStruct.color = meshComp.getColor();
	this->pixelShaderBufferStruct.shade = meshComp.getShouldShade();
	this->pixelShaderConstantBuffer.updateBuffer(&this->pixelShaderBufferStruct);
	immediateContext->PSSetConstantBuffers(2, 1, &this->pixelShaderConstantBuffer.getBuffer());

	// Render submeshes
	for (unsigned int j = 0; j < mesh.getSubmeshes().size(); ++j)
	{
		Submesh& currentSubmesh = mesh.getSubmeshes()[j];

		Material& material = this->resources.getMaterial(
			strcmp(currentSubmesh.materialName, "") ?
			currentSubmesh.materialName :
			meshComp.getMaterialName().c_str()
		);
		Texture& texture = this->resources.getTexture(material.getDiffuseTextureName().c_str());
		immediateContext->PSSetSamplers(
			0, 1, &texture.getSampler()
		);
		immediateContext->PSSetShaderResources(
			0, 1, &texture.getSRV().getPtr()
		);

		immediateContext->DrawIndexed(
			currentSubmesh.numIndices, currentSubmesh.startIndex, 0
		);
	}
}

Renderer::Renderer(Resources& resources)
	: device(nullptr), immediateContext(nullptr), swapChain(nullptr),
	viewport(), backBufferRTV(nullptr), dsTexture(*this),
	dsView("rendererDSV"),

	vertexShader(*this),
	outlineComputeShader(*this, "outlinesComputeShader"),

	cameraConstantBuffer(*this, "cameraConstantBuffer"),
	compactCameraConstantBuffer(*this, "compactCameraConstantBuffer"),
	pixelShaderConstantBuffer(*this, "pixelShaderConstantBuffer"),
	outlineInfoConstantBuffer(*this, "outlineInfoConstantBuffer"),
	backBufferUAV(*this, "backBufferUAV"),

	resources(resources),

	skybox(*this)
{
}

Renderer::~Renderer()
{
	// Switch to windowed mode
	this->swapChain->SetFullscreenState(false, NULL);

	S_RELEASE(this->device);
	S_RELEASE(this->immediateContext);
	S_RELEASE(this->swapChain);

	S_RELEASE(this->backBufferRTV);
	S_RELEASE(this->dsState);
}

void Renderer::init(Window& window)
{
	this->window = &window;

	this->evaluateAdapterModes();
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
	this->pixelShaderConstantBuffer.createBuffer(sizeof(PixelShaderBufferData));
	this->outlineInfoConstantBuffer.createBuffer(sizeof(OutlineInfoBufferData));

	// Init skybox
	this->skybox.initialize();

	// Outlines compute shader
	this->outlineComputeShader.addUAV(this->backBufferUAV);
	this->outlineComputeShader.addSRV(this->dsTexture.getSRV());
	this->outlineComputeShader.addConstantBuffer(this->outlineInfoConstantBuffer);
	this->outlineInfoBufferStruct.width = this->window->getWidth();
	this->outlineInfoBufferStruct.height = this->window->getHeight();
	this->outlineInfoBufferStruct.thickness = 0.001f;

	// Topology won't change during runtime
	immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Renderer::render(Scene& scene)
{
#ifdef _DEBUG
	if (!scene.getActiveCamera())
		Log::error("No active camera has been set in the renderer.");
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

	// --------------------- Render shadow maps ---------------------

	std::vector<Light*> lightComponents = scene.getActiveComponents<Light>();
	
	// ----- Render shadow maps -----
	for (unsigned int i = 0; i < lightComponents.size(); ++i)
	{
		lightComponents[i]->render(scene);
	}
	Light* firstLight = lightComponents.size() > 0 ?
		lightComponents[0] : nullptr;

	// --------------------- Render meshes ---------------------

	immediateContext->RSSetViewports(1, &this->viewport);
	immediateContext->VSSetShader(this->vertexShader.getVS(), nullptr, 0);
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
		MeshComp* currentMeshComp = meshComponents[i];
		this->renderMesh(*currentMeshComp);
	}

	// Remove third constant buffer
	immediateContext->PSSetConstantBuffers(2, 1, this->nullConstantBuffer);

	// --------------------- Render skybox ---------------------

	//Skybox
	immediateContext->VSSetConstantBuffers(0, 1, &this->skybox.getConstantBuffer().getBuffer());

	// Update mvp Matrix
	this->skybox.update(scene.getActiveCamera()->getTransform()->getPosition(), vp);
	
	//Bind skybox
	immediateContext->IASetInputLayout(this->skybox.getVertexShader().getInputLayout());
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

	// --------------------- Render particles ---------------------
	DirectX::SimpleMath::Vector3 cameraPos = scene.getActiveCamera()->getTransform()->getPosition();

	std::vector<ParticleEmitter*> particleComponents = scene.getActiveComponents<ParticleEmitter>();
	for (unsigned int i = 0; i < particleComponents.size(); ++i)
	{
		particleComponents[i]->render(vp, cameraPos);
	}

	// --------------------- Render outlines ---------------------
	immediateContext->OMSetRenderTargets(1, this->nullRTV, nullptr);

	// Update Constant buffer
	this->outlineInfoBufferStruct.projectionInv = scene.getActiveCamera()->getInvProjectionMatrix().Transpose();
	this->outlineInfoConstantBuffer.updateBuffer(&this->outlineInfoBufferStruct);
	this->outlineComputeShader.run();

	immediateContext->OMSetRenderTargets(1, &this->backBufferRTV, this->dsView.getPtr());

	// --------------------- Render absolute meshes ---------------------
	immediateContext->VSSetConstantBuffers(0, 1, &this->cameraConstantBuffer.getBuffer());
	immediateContext->VSSetShader(this->vertexShader.getVS(), nullptr, 0);

	immediateContext->ClearDepthStencilView(this->dsView.getPtr(), D3D11_CLEAR_DEPTH, 1.0f, 0);

	// Render all absolute meshes
	std::vector<AbsoluteMeshComp*> absMeshComponents =
		scene.getActiveComponents<AbsoluteMeshComp>();
	for (unsigned int i = 0; i < absMeshComponents.size(); ++i)
	{
		AbsoluteMeshComp* currentMeshComp = absMeshComponents[i];
		this->renderMesh(*currentMeshComp);
	}

	//Log::write("Num draw calls: " + std::to_string(numDrawCalls) + "  ms: " + std::to_string(Time::getDT() * 1000.0f));

	// Remove third constant buffer
	immediateContext->PSSetConstantBuffers(2, 1, nullConstantBuffer);

	// Remove shadow map from slot 1
	immediateContext->PSSetShaderResources(
		1, 1, this->nullSRV
	);

	// Unbind render target
	immediateContext->OMSetRenderTargets(1, this->nullRTV, nullptr);

	// --------------------- Render outlines ---------------------
	this->outlineComputeShader.run();
}

void Renderer::presentSC()
{
	this->swapChain->Present(1, 0);
}

void Renderer::setSkyBoxName(const std::string& name)
{
	this->skyboxName = name;
}
