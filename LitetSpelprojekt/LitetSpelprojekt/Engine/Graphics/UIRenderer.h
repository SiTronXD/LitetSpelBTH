#pragma once

#include <string>

#include "ConstantBuffer.h"
#include "Shaders/ComputeShader.h"
#include "../Resources.h"

class UIRenderer
{
private:
	struct UIOrientationBufferData
	{
		DirectX::XMINT2 position;
		DirectX::XMINT2 textureSize;
		DirectX::XMINT2 uiSize;
		DirectX::XMFLOAT2 padding2;
	} uiOrientationBufferStruct{};

	unsigned int windowWidth;
	unsigned int windowHeight;

	ConstantBuffer uiOrientationBuffer;

	ComputeShader uiRenderComputeShader;

	Renderer& renderer;
	Resources& resources;

public:
	UIRenderer(Renderer& renderer, Resources& resources);
	virtual ~UIRenderer();

	void init(unsigned int windowWidth, unsigned int windowHeight);

	void renderTexture(
		const std::string& textureName,
		int x, int y, int width, int height
	);
};