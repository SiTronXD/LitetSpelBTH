#pragma once

#include <Windows.h>
#include <d3d11.h>
#include <SimpleMath.h>
#include <fstream>

//#include "../Components/Camera.h"
//#include "../Components/MeshComp.h"
#include "../Application/Scene.h"
#include "ConstantBuffer.h"
#include "Shaders/VertexShader.h"
#include "Shaders/PixelShader.h"
#include "../Application/Window.h"
#include "Texture.h"
#include "../Resources.h"
#include "UAV.h"
#include "Skybox.h"
#include "ParticleSystem.h"

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
	ID3D11DepthStencilState* dsState;

	ConstantBuffer cameraConstantBuffer;

	Window* window;
	Resources& resources;

	UAV backBufferUAV;

	Skybox skybox;
	ParticleSystem particles;

	std::string skyboxName;

	//Camera* activeCamera;

	// Functions
	bool createInterfaces();
	bool createViews();

	bool loadShaders();

public:
	Renderer(Resources& resources);
	virtual ~Renderer();

	void init(Window& window);
	void render(Scene& scene);
	void presentSC();
	void setSkyBoxName(const std::string& name);

	//void setActiveCamera(Camera& camera);

	inline UAV& getBackBufferUAV() { return this->backBufferUAV; }

	inline ID3D11Device* getDevice() const { return this->device; }
	inline ID3D11DeviceContext* getDeviceContext() const { return this->immediateContext; }


};