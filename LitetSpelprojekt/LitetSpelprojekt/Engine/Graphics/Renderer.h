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
#include "DSV.h"
#include "Skybox.h"
#include "ParticleSystem.h"

class Renderer
{
private:
	struct CameraBufferData
	{
		DirectX::XMFLOAT4X4 modelMat;
		DirectX::XMFLOAT4X4 vpMat;
	} cameraBufferStruct{};

	struct CompactCameraBufferData
	{
		DirectX::XMFLOAT4X4 mvpMat;
	} compactCameraBufferStruct{};

	struct PixelShaderBufferData
	{
		DirectX::XMFLOAT3 color;
		int shade;
	} pixelShaderBufferStruct{};

	struct OutlineInfoBufferData
	{
		DirectX::XMFLOAT4X4 projectionInv;
		float width;
		float height;
		float thickness;
		float pad;
	} outlineInfoBufferStruct{};

	ID3D11Device* device;
	ID3D11DeviceContext* immediateContext;
	IDXGISwapChain* swapChain;

	VertexShader vertexShader;
	PixelShader pixelShader;
	ComputeShader outlineComputeShader;

	D3D11_VIEWPORT viewport;
	ID3D11RenderTargetView* backBufferRTV;
	Texture dsTexture;
	ID3D11DepthStencilState* dsState;
	DSV dsView;

	ID3D11Buffer* nullConstantBuffer[1]{ nullptr };
	ID3D11ShaderResourceView* nullSRV[1]{ nullptr };
	ID3D11RenderTargetView* nullRTV[1] = { nullptr };

	ConstantBuffer cameraConstantBuffer;
	ConstantBuffer compactCameraConstantBuffer;
	ConstantBuffer pixelShaderConstantBuffer;
	ConstantBuffer outlineInfoConstantBuffer;

	Window* window;
	Resources& resources;

	UAV backBufferUAV;

	Skybox skybox;

	std::string skyboxName;

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
	inline ConstantBuffer& getCompactCameraConstantBuffer() { return this->compactCameraConstantBuffer; }
	inline ConstantBuffer& getCameraConstantBuffer() { return this->cameraConstantBuffer; }
	inline CameraBufferData& getCameraBufferStruct() { return this->cameraBufferStruct; }
	inline CompactCameraBufferData& getCompactCameraBufferStruct() { return this->compactCameraBufferStruct; }
};