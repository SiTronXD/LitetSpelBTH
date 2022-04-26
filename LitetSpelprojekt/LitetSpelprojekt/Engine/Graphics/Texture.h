#pragma once

#include <string>
#include <d3d11.h>
#include <DirectXMath.h>

#include "SRV.h"

class Renderer;

class Texture
{
private:
	ID3D11SamplerState* samplerState;
	ID3D11Texture2D* texture;

	D3D11_TEXTURE2D_DESC textureDesc;

	SRV textureSRV;

	Renderer& renderer;

	unsigned char* pixels;

	int width;
	int height;
	int numChannels;

	bool createSampler(
		const D3D11_FILTER& filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR
	);

public:
	Texture(Renderer& renderer);
	virtual ~Texture();

	bool load(const std::string& fileName, bool saveImageData);
	bool createAsDepthTexture(
		int width, int height,
		const DXGI_FORMAT& format,
		const UINT& additionalBindFlags = 0,
		bool shouldCreateSampler = false
	);
	bool createCubemap(std::string fileName, std::string format);

	DirectX::XMFLOAT4 getPixel(int x, int y);

	inline SRV& getSRV() { return this->textureSRV; }
	inline ID3D11SamplerState*& getSampler() { return this->samplerState; }
	inline ID3D11Texture2D*& getPtr() { return this->texture; }
	inline bool createShaderResourceView() { return this->textureSRV.createTextureSRV(this->texture, textureDesc.Format); }

	inline const unsigned int& getWidth() { return this->width; }
	inline const unsigned int& getHeight() { return this->height; }
};