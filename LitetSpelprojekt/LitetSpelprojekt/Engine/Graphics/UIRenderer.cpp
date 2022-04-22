#include "UIRenderer.h"
#include "Renderer.h"
#include "../ResTranslator.h"

UIRenderer::UIRenderer(
	Renderer& renderer, Resources& resources)
	: renderer(renderer),
	resources(resources),
	uiOrientationBuffer(renderer, "uiOrientationBuffer"),
	uiRenderComputeShader(renderer, "uiRenderComputeShader")
{	

}

UIRenderer::~UIRenderer()
{
}

void UIRenderer::init(unsigned int windowWidth, unsigned int windowHeight)
{
	this->windowWidth = windowWidth;
	this->windowHeight = windowHeight;

	// Init constant buffer
	this->uiOrientationBuffer.createBuffer(sizeof(UIOrientationBufferData));

	// UI compute shader
	this->uiRenderComputeShader.init("RenderUI_COMP", this->windowWidth, this->windowHeight, 1);
	this->uiRenderComputeShader.addConstantBuffer(this->uiOrientationBuffer);

	this->uiRenderComputeShader.addUAV(renderer.getBackBufferUAV());

	this->uiRenderComputeShader.setNumSRVs(1);
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
