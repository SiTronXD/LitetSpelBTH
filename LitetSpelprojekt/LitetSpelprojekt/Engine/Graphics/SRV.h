#pragma once

#include <string>
#include <d3d11.h>

class Renderer;

class SRV
{
private:
	ID3D11ShaderResourceView* resourceSRV;

	Renderer& renderer;

	std::string debugName;

public:
	SRV(Renderer& renderer, const std::string& debugName);
	virtual ~SRV();

	bool createBufferSRV(
		ID3D11Resource* resource, 
		const DXGI_FORMAT& format, 
		const unsigned int& numElements
	);
	bool createTextureSRV(
		ID3D11Resource* resource, const DXGI_FORMAT& format, const D3D11_SRV_DIMENSION& dimension = D3D11_SRV_DIMENSION_TEXTURE2D
	);

	inline ID3D11ShaderResourceView*& getPtr() { return this->resourceSRV; }
	inline const std::string& getDebugName() const { return this->debugName; }
};