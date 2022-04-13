#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "Texture.h"
#include "../Dev/Helpers.h"
#include "../Dev/Log.h"
#include "Renderer.h"

bool Texture::createSampler()
{
	// Sampler description
	D3D11_SAMPLER_DESC samplerDesc{};
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;

	// Create sampler
	HRESULT hr = this->renderer.getDevice()->CreateSamplerState(
		&samplerDesc, &this->samplerState
	);
	if (FAILED(hr))
	{
		Log::error("Could not create texture sampler.");

		return false;
	}

	return true;
}

Texture::Texture(Renderer& renderer)
	: renderer(renderer),
	textureSRV(renderer, "textureSRV"),
	samplerState(nullptr),
	texture(nullptr),
	textureDesc{},
	width(0),
	height(0)
{
}

Texture::~Texture()
{
	S_RELEASE(this->samplerState);
	S_RELEASE(this->texture);
}

bool Texture::load(const std::string& fileName)
{
	S_RELEASE(this->samplerState);
	S_RELEASE(this->texture);

	// Create sampler
	this->createSampler();

	// Load image
	int imageWidth = 0;
	int imageHeight = 0;
	int imageChannels = 0;
	int imageDesiredChannels = 4;
	unsigned char* imageData = stbi_load(
		fileName.c_str(),
		&imageWidth, &imageHeight,
		&imageChannels, imageDesiredChannels
	);
	int imagePitch = imageWidth * imageDesiredChannels;
	if (!imageData)
	{
		Log::error("Failed to load image: " + fileName);

		return false;
	}

	// Save size
	this->width = imageWidth;
	this->height = imageHeight;

	// D3D texture description
	this->textureDesc = {};
	this->textureDesc.Width = imageWidth;
	this->textureDesc.Height = imageHeight;
	this->textureDesc.MipLevels = 1;
	this->textureDesc.ArraySize = 1;
	this->textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	this->textureDesc.SampleDesc.Count = 1;
	this->textureDesc.SampleDesc.Quality = 0;
	this->textureDesc.Usage = D3D11_USAGE_IMMUTABLE;
	this->textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

	// Texture subresource data
	D3D11_SUBRESOURCE_DATA textureSubresourceData{};
	textureSubresourceData.pSysMem = imageData;
	textureSubresourceData.SysMemPitch = imagePitch;

	// Create D3D texture
	HRESULT hr = this->renderer.getDevice()->CreateTexture2D(&this->textureDesc, &textureSubresourceData, &this->texture);

	// Free image data
	stbi_image_free(imageData);

	if (FAILED(hr))
	{
		Log::error("Failed to create texture.");
		return false;
	}

	// Create texture SRV
	return this->createShaderResourceView();
}
