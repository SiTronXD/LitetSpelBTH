#include "UIRenderer.h"
#include "Renderer.h"
#include "../ResTranslator.h"

UIRenderer::UIRenderer(
	Renderer& renderer, Resources& resources)
	: renderer(renderer),
	resources(resources),
	uiOrientationBuffer(renderer, "uiOrientationBuffer"),
	textOrientationBuffer(renderer, "textOrientationBuffer"),
	uiRenderComputeShader(renderer, "uiRenderComputeShader"),
	textRenderComputeShader(renderer, "textRenderComputeShader")
{	

}

UIRenderer::~UIRenderer()
{
}

void UIRenderer::init(unsigned int windowWidth, unsigned int windowHeight)
{
	this->windowWidth = windowWidth;
	this->windowHeight = windowHeight;

	// Init constant buffers
	this->uiOrientationBuffer.createBuffer(sizeof(UIOrientationBufferData));
	this->textOrientationBuffer.createBuffer(sizeof(TextOrientationBufferData));

	// UI compute shader
	this->uiRenderComputeShader.init("RenderUI_COMP", this->windowWidth, this->windowHeight, 1);
	this->uiRenderComputeShader.addConstantBuffer(this->uiOrientationBuffer);
	this->uiRenderComputeShader.addUAV(renderer.getBackBufferUAV());
	this->uiRenderComputeShader.setNumSRVs(1);

	// Text compute shader
	this->textRenderComputeShader.init("RenderTextUI_COMP", this->windowWidth, this->windowHeight, 1);
	this->textRenderComputeShader.addConstantBuffer(this->textOrientationBuffer);
	this->textRenderComputeShader.addUAV(renderer.getBackBufferUAV());
	this->textRenderComputeShader.setNumSRVs(1);
}

void UIRenderer::setFontTexture(const std::string& fontTextureName)
{
	this->fontTextureName = fontTextureName;

	// Load texture
	Texture& texture = this->resources.getTexture(
		this->fontTextureName.c_str()
	);
	unsigned int textureWidth = texture.getWidth();
	unsigned int textureHeight = texture.getHeight();

	// Set texture SRV
	this->textRenderComputeShader.setSRV(0, texture.getSRV());

	// Orientation
	this->textOrientationBufferStruct.textureSize.x = textureWidth;
	this->textOrientationBufferStruct.textureSize.y = textureHeight;
}

void UIRenderer::setFontCharacterOrder(
	const std::vector<std::string>& characters,
	unsigned int tileWidth, unsigned int tileHeight)
{
	this->fontTileWidth = tileWidth;
	this->fontTileHeight = tileHeight;

	// Clear previous characters
	this->fontCharacters.clear();

	// Insert each character
	for (unsigned int row = 0; row < characters.size(); ++row)
	{
		for (unsigned int col = 0; col < characters[row].length(); ++col)
		{
			this->fontCharacters.insert(
				std::pair<char, CharacterRect>(
					characters[row][col],
					CharacterRect
					{ 
						col * tileWidth, 
						row * tileHeight, 
						tileWidth, 
						tileHeight
					}
				)
			);
		}
	}
}

void UIRenderer::renderTexture(
	const std::string& textureName, 
	int x, int y, int uiWidth, int uiHeight)
{
	Texture& texture = this->resources.getTexture(textureName.c_str());
	unsigned int textureWidth = texture.getWidth();
	unsigned int textureHeight = texture.getHeight();

	// Set texture SRV
	this->uiRenderComputeShader.setSRV(0, texture.getSRV());

	// Transform from internal resolution to screen resolution
	UIRectangle transformedRect = ResTranslator::transformRect(
		UIRectangle{ x, y, uiWidth, uiHeight }
	);

	// Orientation
	this->uiOrientationBufferStruct.position.x = transformedRect.x;
	this->uiOrientationBufferStruct.position.y = transformedRect.y;
	this->uiOrientationBufferStruct.uiSize.x = transformedRect.width;
	this->uiOrientationBufferStruct.uiSize.y = transformedRect.height;
	this->uiOrientationBufferStruct.textureSize.x = textureWidth;
	this->uiOrientationBufferStruct.textureSize.y = textureHeight;
	this->uiOrientationBuffer.updateBuffer(&this->uiOrientationBufferStruct);

	// Set the number of thread groups to the resolution
	this->uiRenderComputeShader.setThreadGroup(
		transformedRect.width, transformedRect.height, 1
	);

	// Run
	this->uiRenderComputeShader.run();
}

void UIRenderer::renderString(
	const std::string& text, 
	int x, int y, int characterWidth, int characterHeight)
{
	// Render each character
	int posX = x;
	for (int i = 0; i < text.length(); ++i)
	{
		// Transform from internal resolution to screen resolution
		UIRectangle transformedRect = ResTranslator::transformRect(
			UIRectangle
			{ 
				x + i * characterWidth, 
				y, 
				characterWidth, 
				characterHeight 
			}
		);

		CharacterRect& charRect = this->fontCharacters[text[i]];

		// Orientation per character
		this->textOrientationBufferStruct.position.x = transformedRect.x;
		this->textOrientationBufferStruct.position.y = transformedRect.y;
		this->textOrientationBufferStruct.uiSize.x = transformedRect.width;
		this->textOrientationBufferStruct.uiSize.y = transformedRect.height;
		this->textOrientationBufferStruct.cutoutRectPos.x = charRect.x;
		this->textOrientationBufferStruct.cutoutRectPos.y = charRect.y;
		this->textOrientationBufferStruct.cutoutRectSize.x = charRect.tileWidth;
		this->textOrientationBufferStruct.cutoutRectSize.y = charRect.tileHeight;
		this->textOrientationBuffer.updateBuffer(&this->textOrientationBufferStruct);

		// Set the number of thread groups to the resolution
		this->textRenderComputeShader.setThreadGroup(
			transformedRect.width, transformedRect.height, 1
		);

		// Run
		this->textRenderComputeShader.run();

		// Move character
		posX += (float) characterWidth * (charRect.tileWidth / this->fontTileWidth);
	}
}
