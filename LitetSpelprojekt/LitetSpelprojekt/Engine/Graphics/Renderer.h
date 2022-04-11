#pragma once

#include <Windows.h>
#include <d3d11.h>
#include <SimpleMath.h>
#include <fstream>

#include "../Components/Camera.h"
#include "../Components/MeshComp.h"
#include "ConstantBuffer.h"
#include "Shaders/VertexShader.h"
#include "Shaders/PixelShader.h"
#include "../Application/Window.h"
#include "Texture.h"

class Renderer
{
private:
	struct CameraBufferData
	{
		DirectX::XMFLOAT4X4 mvpMat;
	} cameraBufferStruct{};

	ID3D11Device* device;
	ID3D11DeviceContext* immediateContext;
	IDXGISwapChain* swapChain;

	VertexShader vertexShader;
	PixelShader pixelShader;

	D3D11_VIEWPORT viewport;
	ID3D11RenderTargetView* backBufferRTV;
	ID3D11Texture2D* dsTexture;
	ID3D11DepthStencilView* dsView;

	ConstantBuffer cameraConstantBuffer;

	Texture testTexture;

	// Functions
	bool createInterfaces(Window& window);
	bool createViews(Window& window);

	bool loadShaders();
public:
	Renderer();
	virtual ~Renderer();

	void init(Window& window);
	void render(Camera& camera, std::vector<MeshComp*>& meshComponents);
	void presentSC();

	inline ID3D11Device* getDevice() const { return this->device; }
	inline ID3D11DeviceContext* getDeviceContext() const { return this->immediateContext; }
};