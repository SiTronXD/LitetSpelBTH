#include "StructuredBuffer.h"

StructuredBuffer::StructuredBuffer(Renderer& renderer, const std::string& debugName)
	: Buffer(renderer, debugName),
	srv(renderer, "Structured Buffer SRV")
{
}

StructuredBuffer::~StructuredBuffer()
{
}

void StructuredBuffer::updateBuffer(void* bufferData)
{
	D3D11_MAPPED_SUBRESOURCE mappedSubresource;
	Buffer::map(mappedSubresource);

	// Copy over data
	memcpy(mappedSubresource.pData, bufferData, Buffer::getBufferSize());

	Buffer::unmap();
}

bool StructuredBuffer::createBuffer(
	UINT elementSize, UINT numElements,
	void* initialData)
{
	bool success = Buffer::createBuffer(
		D3D11_USAGE_DEFAULT,
		D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE,
		elementSize * numElements,
		initialData,
		0,
		D3D11_RESOURCE_MISC_BUFFER_STRUCTURED,
		elementSize
	);

	success = this->srv.createBufferSRV(
		this->getBuffer(), DXGI_FORMAT_UNKNOWN, numElements
	);

	return success;
}
