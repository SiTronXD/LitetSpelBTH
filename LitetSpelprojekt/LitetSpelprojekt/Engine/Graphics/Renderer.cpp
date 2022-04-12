#include "Renderer.h"
#include "../Dev/Log.h"
#include "../Time.h"

using namespace DirectX::SimpleMath;

bool Renderer::createInterfaces(Window& window)
{
	UINT flags = 0;
#ifdef _DEBUG
	flags = D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_0 };

	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};

	swapChainDesc.BufferDesc.Width = window.getWidth();
	swapChainDesc.BufferDesc.Height = window.getHeight();
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	swapChainDesc.BufferUsage = DXGI_USAGE_UNORDERED_ACCESS | DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 1;
	swapChainDesc.OutputWindow = window.getWindowHandle();
	swapChainDesc.Windowed = true;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = 0;

	return !(FAILED(D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, flags, featureLevels, 1, D3D11_SDK_VERSION,
		&swapChainDesc, &this->swapChain, &this->device, nullptr, &this->immediateContext)));
}

bool Renderer::createViews(Window& window)
{
	// Get address of the back buffer
	ID3D11Texture2D* backBuffer = nullptr;
	if (FAILED(swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer))))
	{
		Log::error("Backbuffer Texture not found.");
		return false;
	}

	if (FAILED(device->CreateRenderTargetView(backBuffer, NULL, &this->backBufferRTV)))
	{
		Log::error("Failed to create backbufferRTV");
	}
	backBuffer->Release();

	D3D11_TEXTURE2D_DESC textureDesc;
	textureDesc.Width = window.getWidth();
	textureDesc.Height = window.getHeight();
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;
	if (FAILED(device->CreateTexture2D(&textureDesc, nullptr, &dsTexture)))
	{
		Log::error("Failed to create Depth Stencil Texture");
		return false;
	}

	if (FAILED(device->CreateDepthStencilView(dsTexture, 0, &dsView)))
	{
		Log::error("Failed to create Depth Stencil View");
		return false;
	}

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
	this->vertexShader.loadVS("VertexShader", inputLayoutDesc);

	// Pixel shader
	this->pixelShader.loadPS("PixelShader");

	return true;
}

Renderer::Renderer(Resources& resources)
	: device(nullptr), immediateContext(nullptr), swapChain(nullptr),
	viewport(), backBufferRTV(nullptr), dsTexture(nullptr), dsView(nullptr),

	vertexShader(*this),
	pixelShader(*this),

	cameraConstantBuffer(*this, "cameraConstantBuffer"),

	resources(resources)
{
}

Renderer::~Renderer()
{
	this->device->Release();
	this->immediateContext->Release();
	this->swapChain->Release();

	this->backBufferRTV->Release();
	this->dsTexture->Release();
	this->dsView->Release();
}

void Renderer::init(Window& window)
{
	createInterfaces(window);
	createViews(window);
	loadShaders();

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = static_cast<float>(window.getWidth());
	viewport.Height = static_cast<float>(window.getHeight());
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1;

	// Constant buffer
	this->cameraConstantBuffer.createBuffer(sizeof(CameraBufferData));
}

float timer = 0.0f;
void Renderer::render(Camera& camera, std::vector<MeshComp*>& meshComponents)
{
	// Update camera constant buffer
	timer += Time::getDT();
	Matrix m;
	m *= Matrix::CreateRotationY(timer);
	m *= camera.getViewMatrix();
	m *= camera.getProjectionMatrix();
	this->cameraBufferStruct.mvpMat = m.Transpose();
	this->cameraConstantBuffer.updateBuffer(&this->cameraBufferStruct);
	immediateContext->VSSetConstantBuffers(0, 1, &this->cameraConstantBuffer.getBuffer());

	// Init setup
	immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	immediateContext->RSSetViewports(1, &this->viewport);
	immediateContext->OMSetRenderTargets(1, &this->backBufferRTV, this->dsView);
	immediateContext->VSSetShader(this->vertexShader.getVS(), nullptr, 0);
	immediateContext->PSSetShader(this->pixelShader.getPS(), nullptr, 0);

	// Clear buffers
	float clearColour[4] = { 0, 0, 0, 0 };
	immediateContext->ClearRenderTargetView(this->backBufferRTV, clearColour);
	immediateContext->ClearDepthStencilView(this->dsView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);

	// Render all meshes
	for (unsigned int i = 0; i < meshComponents.size(); ++i)
	{
		Mesh& mesh = this->resources.getMesh(meshComponents[i]->getMeshName());

		// Set texture
		Material& material = this->resources.getMaterial(meshComponents[i]->getMaterialName());
		Texture& texture = this->resources.getTexture(material.getDiffuseTextureName());
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

		immediateContext->DrawIndexed(
			mesh.getIndexBuffer().getIndexCount(), 0, 0
		);
	}
}

void Renderer::presentSC()
{
	this->swapChain->Present(1, 0);
}
