#pragma once

#include <d3d11.h>
#include <directxmath.h>
#include <fstream>

#include "../Application/Window.h"

struct SimpleVertex
{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 normal;
	DirectX::XMFLOAT2 uv;

	SimpleVertex(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 normal, DirectX::XMFLOAT2 uv):
		pos(pos), normal(normal), uv(uv)
	{ }
};

class Renderer
{
private:
	ID3D11Device* device;
	ID3D11DeviceContext* immediateContext;
	IDXGISwapChain* swapChain;

	ID3D11VertexShader* vertexShader;
	ID3D11InputLayout* inputLayout;
	ID3D11PixelShader* pixelShader;

	D3D11_VIEWPORT viewport;
	ID3D11RenderTargetView* backBufferRTV;
	ID3D11Texture2D* dsTexture;
	ID3D11DepthStencilView* dsView;

	ID3D11Buffer* vertexBuffer;

	// Functions
	bool createInterfaces(Window& window);
	bool createViews(Window& window);

	std::string loadShaderData(std::string path);
	bool loadShaders();

	bool createTriangle();
public:
	Renderer();
	virtual ~Renderer();

	void init(Window& window);
	void render();
	void presentSC();
};

