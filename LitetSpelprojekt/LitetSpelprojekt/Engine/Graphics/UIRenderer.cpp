#include "UIRenderer.h"
#include "Renderer.h"
#include "../ResTranslator.h"

using namespace DirectX;

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
	if (this->fontTextureName == "")
	{
		Log::error("You need to set font texture name before character order.");

		return;
	}

	Texture& texture = this->resources.getTexture(
		this->fontTextureName.c_str()
	);

	this->fontTileWidth = tileWidth;
	this->fontTileHeight = tileHeight;

	// Clear previous characters
	this->fontCharacters.clear();

	// Insert each character
	for (unsigned int row = 0; row < characters.size(); ++row)
	{
		for (unsigned int col = 0; col < characters[row].length(); ++col)
		{
			CharacterRect newCharRect
			{
				col * tileWidth,
				row * tileHeight,
				tileWidth,
				tileHeight
			};

			// Find bounds
			int xMax = 0;
			int yMax = 0;
			int xMin = tileWidth;
			int yMin = tileHeight;
			for(int yo = 0; yo < tileHeight; ++yo)
			{
				for (int xo = 0; xo < tileWidth; ++xo)
				{
					XMFLOAT4 pixel = texture.getPixel(newCharRect.x + xo, newCharRect.y + yo);

					// Update bounds
					if (pixel.x + pixel.y + pixel.z + pixel.w > 0)
					{
						if (xo > xMax) xMax = xo;
						if (yo > yMax) yMax = yo;
						if (xo < xMin) xMin = xo;
						if (yo < yMin) yMin = yo;
					}
				}
			}
			// Set new bounds
			if (xMax >= xMin && yMax >= yMin)
			{
				newCharRect.x += xMin;
				newCharRect.y += yMin;
				newCharRect.tileWidth = xMax - xMin + 1;
				newCharRect.tileHeight = yMax - yMin + 1;
			}

			this->fontCharacters.insert(
				std::pair<char, CharacterRect>(
					characters[row][col],
					newCharRect
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
	// Get entire text width
	int textWidth = 0;
	for (int i = 0; i < text.length(); ++i)
	{
		// Get character size
		CharacterRect& charRect = this->fontCharacters[text[i]];

		float sizeRatioX = (float) charRect.tileWidth / this->fontTileWidth;

		textWidth += (int) (characterWidth * sizeRatioX);
	}

	int posX = x - (textWidth / 2);

	// Render each character
	for (int i = 0; i < text.length(); ++i)
	{
		// Get character size
		CharacterRect& charRect = this->fontCharacters[text[i]];

		float sizeRatioX = (float) charRect.tileWidth / this->fontTileWidth;
		float sizeRatioY = (float) charRect.tileHeight / this->fontTileHeight;

		// Move character from edge to middle
		posX += (int)(characterWidth * sizeRatioX * 0.5f);

		// Transform from internal resolution to screen resolution
		UIRectangle transformedRect = ResTranslator::transformRect(
			UIRectangle
			{ 
				posX, 
				y, 
				(int) (characterWidth * sizeRatioX), 
				(int) (characterHeight * sizeRatioY)
			}
		);

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

		// Move character from middle to edge
		posX += (int) (characterWidth * sizeRatioX * 0.5f);
	}
}
