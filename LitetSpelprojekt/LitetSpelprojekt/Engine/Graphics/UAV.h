#pragma once

#include <string>
#include <d3d11.h>

class Renderer;

class UAV
{
private:
	ID3D11UnorderedAccessView* bufferUAV;

	Renderer& renderer;

	std::string debugName;

public:
	UAV(Renderer& renderer, const std::string& debugName);
	virtual ~UAV();

	bool createBufferUAV(ID3D11Resource* buffer, const DXGI_FORMAT& format, const unsigned int& numElements);

	bool createTextureUAV(
		ID3D11Resource* buffer, const DXGI_FORMAT& format
	);

	inline ID3D11UnorderedAccessView*& getUAVObject() { return this->bufferUAV; }
	inline const std::string& getDebugName() const { return this->debugName; }
};
