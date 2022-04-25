#pragma once

#include <unordered_map>
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

	struct TextOrientationBufferData
	{
		DirectX::XMINT2 position;
		DirectX::XMINT2 textureSize;
		DirectX::XMINT2 uiSize;
		DirectX::XMINT2 cutoutRectPos;
		DirectX::XMINT2 cutoutRectSize;

		DirectX::XMFLOAT2 padding1;
	} textOrientationBufferStruct{};

	struct CharacterRect
	{
		unsigned int x;
		unsigned int y;
		unsigned int tileWidth;
		unsigned int tileHeight;
	};

	unsigned int windowWidth;
	unsigned int windowHeight;
	unsigned int fontTileWidth;
	unsigned int fontTileHeight;

	std::unordered_map<char, CharacterRect> fontCharacters;
	std::string fontTextureName;

	ConstantBuffer uiOrientationBuffer;
	ConstantBuffer textOrientationBuffer;

	ComputeShader uiRenderComputeShader;
	ComputeShader textRenderComputeShader;

	Renderer& renderer;
	Resources& resources;

public:
	UIRenderer(Renderer& renderer, Resources& resources);
	virtual ~UIRenderer();

	void init(unsigned int windowWidth, unsigned int windowHeight);
	void setFontTexture(const std::string& fontTextureName);
	void setFontCharacterOrder(
		const std::vector<std::string>& characters,
		unsigned int tileWidth,
		unsigned int tileHeight
	);

	void renderTexture(
		const std::string& textureName,
		int x, int y, int width, int height
	);
	void renderString(
		const std::string& text,
		int x, int y, int characterWidth, int characterHeight
	);
};