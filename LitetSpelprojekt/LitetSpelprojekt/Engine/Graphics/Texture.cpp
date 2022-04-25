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
	pixels(nullptr),
	width(0),
	height(0),
	numChannels(0)
{
}

Texture::~Texture()
{
	if (this->pixels)
	{
		stbi_image_free(this->pixels);
		this->pixels = nullptr;
	}

	S_RELEASE(this->samplerState);
	S_RELEASE(this->texture);
}

bool Texture::load(const std::string& fileName, bool saveImageData)
{
	S_RELEASE(this->samplerState);
	S_RELEASE(this->texture);

	// Create sampler
	this->createSampler();

	// Load image
	int imageDesiredChannels = 4;
	unsigned char* imageData = stbi_load(
		fileName.c_str(),
		&this->width, &this->height,
		&this->numChannels, imageDesiredChannels
	);
	int imagePitch = this->width * imageDesiredChannels;
	if (!imageData)
	{
		Log::error("Failed to load image: " + fileName);

		return false;
	}

	// D3D texture description
	this->textureDesc = {};
	this->textureDesc.Width = this->width;
	this->textureDesc.Height = this->height;
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
	if (!saveImageData)
		stbi_image_free(imageData);
	else
		this->pixels = imageData;

	if (FAILED(hr))
	{
		Log::error("Failed to create texture.");
		return false;
	}

	// Create texture SRV
	return this->createShaderResourceView();
}

bool Texture::createCubemap(std::string fileName, std::string format)
{
	HRESULT hr = {};
	
	//Release old texture/data if exist.
	S_RELEASE(this->samplerState);
	S_RELEASE(this->texture);

	// Create sampler
	this->createSampler();

	// Load all six images and set Texture subresource data
	int imageWidth = 0;
	int imageHeight = 0;
	int imageChannels = 0;
	int imageDesiredChannels = 4;

	unsigned char* imageData[6] = {};
	D3D11_SUBRESOURCE_DATA textureSubresourceData[6] = {};

	int imagePitch = 0;

	for (int i = 0; i < 6; i++)
	{
		//Load image
		std::string tempPath = "Resources/Textures/CubeMap/"+ fileName + std::to_string(i + 1) + format;
		
		imageData[i] = stbi_load(
			tempPath.c_str(),
			&imageWidth, &imageHeight,
			&imageChannels, imageDesiredChannels
		);

		if (!imageData[i])
		{
			Log::error("Failed to load image: " + fileName + std::to_string(i) + format);
			return false;
			break;
		}

		imagePitch = imageWidth * imageDesiredChannels;

		//Texture subresource data
		textureSubresourceData[i].pSysMem = imageData[i];
		textureSubresourceData[i].SysMemPitch = imagePitch;
		textureSubresourceData[i].SysMemSlicePitch = 0;
	}

	// Create texture
	this->textureDesc = {};
	this->textureDesc.Width = imageWidth;
	this->textureDesc.Height = imageHeight;
	this->textureDesc.MipLevels = 1;
	this->textureDesc.ArraySize = 6;
	this->textureDesc.CPUAccessFlags = 0;
	this->textureDesc.SampleDesc.Count = 1;
	this->textureDesc.SampleDesc.Quality = 0;
	this->textureDesc.Usage = D3D11_USAGE_DEFAULT;
	this->textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	this->textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	this->textureDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

	// Create D3D texture
	hr = this->renderer.getDevice()->CreateTexture2D(&this->textureDesc, textureSubresourceData, &this->texture);

	// Free image data
	for (int i = 0; i < 6; i++)
	{	
		stbi_image_free(imageData[i]);
	}

	if (FAILED(hr))
	{
		Log::error("Failed to create cubemap texture.");
		return false;
	}

	// Create texture SRV
	return this->textureSRV.createTextureSRV(this->texture, textureDesc.Format, D3D11_SRV_DIMENSION_TEXTURECUBE);

	// Free image data
	stbi_image_free(imageData);
}

DirectX::XMFLOAT4 Texture::getPixel(int x, int y)
{
	return DirectX::XMFLOAT4(
		(float) this->pixels[
			this->width * this->numChannels * y + 
			this->numChannels * x + 0
		],
		(float)this->pixels[
			this->width * this->numChannels * y +
			this->numChannels * x + 1
		],
				(float)this->pixels[
			this->width * this->numChannels * y +
			this->numChannels * x + 2
		],
		(float) this->pixels[
			this->width * this->numChannels * y +
			this->numChannels * x + 3
		]
	);
}
