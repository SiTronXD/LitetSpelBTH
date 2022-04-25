#pragma once

#include <string>
#include <d3d11.h>

class Renderer;

class DSV
{
private:
	ID3D11DepthStencilView* bufferDSV;

	Renderer& renderer;

	std::string debugName;

public:
	DSV(Renderer& renderer, const std::string& debugName);
	~DSV();

	void clear();

	bool createDSV(
		ID3D11Resource* buffer, const DXGI_FORMAT& format
	);

	inline ID3D11DepthStencilView*& getDSVObject() { return this->bufferDSV; }
};