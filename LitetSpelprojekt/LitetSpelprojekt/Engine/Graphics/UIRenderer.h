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

		DirectX::XMFLOAT4 color;
	} uiOrientationBufferStruct{};

	struct TextOrientationBufferData
	{
		DirectX::XMINT2 position;
		DirectX::XMINT2 textureSize;
		DirectX::XMINT2 uiSize;
		DirectX::XMINT2 cutoutRectPos;
		DirectX::XMINT2 cutoutRectSize;

		DirectX::XMFLOAT2 padding1;

		DirectX::XMFLOAT4 color;
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

	int characterSpace;

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
	void setFontCharacterSpacing(int characterSpace);
	void setFontSpaceWidth(int spaceWidth);

	void renderTexture(
		const std::string& textureName,
		int x, int y, int uiWidth, int uiHeight,
		const DirectX::SimpleMath::Vector3& color =
			DirectX::SimpleMath::Vector3(1, 1, 1)
	);
	void renderTexture(
		SRV& srv,
		int x, int y, int uiWidth, int uiHeight,
		int srvWidth, int srvHeight,
		const DirectX::SimpleMath::Vector3& color =
			DirectX::SimpleMath::Vector3(1, 1, 1)
	);
	void renderString(
		const std::string& text,
		int x, int y, int characterWidth, int characterHeight,
		const DirectX::SimpleMath::Vector3& color = 
			DirectX::SimpleMath::Vector3(1,1,1)
	);
};