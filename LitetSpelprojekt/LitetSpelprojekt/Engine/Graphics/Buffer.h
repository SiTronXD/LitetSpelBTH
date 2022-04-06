#pragma once

#include <d3d11.h>

class Renderer;

class Buffer
{
private:
	ID3D11Buffer* buffer;

	UINT bufferSize;

	Renderer& renderer;

protected:
	void map(D3D11_MAPPED_SUBRESOURCE& outputMappedSubresource);
	void unmap();

	bool createBuffer(
		D3D11_USAGE usage, UINT bindFlags, UINT bufferDataSize,
		void* initialData,
		UINT cpuAccessFlags = 0,
		UINT miscFlags = 0,
		UINT structureByteStride = 0);

public:
	Buffer(Renderer& renderer);
	virtual ~Buffer();

	inline ID3D11Buffer*& getBuffer() { return this->buffer; }

	inline const UINT& getBufferSize() { return this->bufferSize; }
};