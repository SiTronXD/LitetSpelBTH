#include "Renderer.h"
#include "../Dev/Log.h"

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

std::string Renderer::loadShaderData(std::string path)
{
	std::string shaderData;
	std::ifstream reader;

	reader.open("CompiledShaders/" + path, std::ios::binary | std::ios::ate);
	if (!reader.is_open())
	{
		Log::error("Shader file: " + path + " not found.");
		return std::string();
	}

	reader.seekg(0, std::ios::end);
	shaderData.reserve(static_cast<unsigned int>(reader.tellg()));
	reader.seekg(0, std::ios::beg);

	shaderData.assign((std::istreambuf_iterator<char>(reader)),
		std::istreambuf_iterator<char>());
	reader.close();
	return shaderData;
}

bool Renderer::loadShaders()
{
	std::string shaderData = loadShaderData("VertexShader.cso");
	if (FAILED(device->CreateVertexShader(shaderData.c_str(), shaderData.length(), nullptr, &this->vertexShader)))
	{
		Log::error("Failed to create: Vertex Shader");
		return false;
	}
	D3D11_INPUT_ELEMENT_DESC inputDesc[3] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
	if (FAILED(device->CreateInputLayout(inputDesc, 3, shaderData.c_str(), shaderData.length(), &this->inputLayout)))
	{
		Log::error("Failed to create: Input Layout");
		return false;
	}
	shaderData.clear();

	shaderData = loadShaderData("PixelShader.cso");
	if (FAILED(device->CreatePixelShader(shaderData.c_str(), shaderData.length(), nullptr, &this->pixelShader)))
	{
		Log::error("Failed to create: Pixel Shader");
		return false;
	}
	shaderData.clear();

	return true;
}

bool Renderer::createTriangle()
{
	MeshData meshData;
	this->vertexBuffer.createBuffer(meshData);
	return this->indexBuffer.createBuffer(meshData);
}

Renderer::Renderer():
	device(nullptr), immediateContext(nullptr), swapChain(nullptr),
	vertexShader(nullptr), inputLayout(nullptr), pixelShader(nullptr), 
	viewport(), backBufferRTV(nullptr), dsTexture(nullptr), dsView(nullptr),

	vertexBuffer(*this),
	indexBuffer(*this)
{
}

Renderer::~Renderer()
{
	this->device->Release();
	this->immediateContext->Release();
	this->swapChain->Release();

	this->vertexShader->Release();
	this->inputLayout->Release();
	this->pixelShader->Release();

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

	createTriangle();
}

void Renderer::render()
{
	float clearColour[4] = { 0, 0, 0, 0 };
	immediateContext->ClearRenderTargetView(this->backBufferRTV, clearColour);
	immediateContext->ClearDepthStencilView(this->dsView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);

	immediateContext->IASetVertexBuffers(
		0, 1, &this->vertexBuffer.getBuffer(), &this->vertexBuffer.getStride(), &this->vertexBuffer.getOffset());
	immediateContext->IASetIndexBuffer(
		this->indexBuffer.getBuffer(), DXGI_FORMAT_R32_UINT, 0
	);
	immediateContext->IASetInputLayout(this->inputLayout);
	immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	immediateContext->VSSetShader(this->vertexShader, nullptr, 0);
	immediateContext->RSSetViewports(1, &this->viewport);
	immediateContext->PSSetShader(this->pixelShader, nullptr, 0);
	immediateContext->OMSetRenderTargets(1, &this->backBufferRTV, this->dsView);

	immediateContext->DrawIndexed(
		this->indexBuffer.getIndexCount(), 0, 0
	);
}

void Renderer::presentSC()
{
	this->swapChain->Present(0, 0);
}
