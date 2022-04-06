#include "StructuredBuffer.h"

StructuredBuffer::StructuredBuffer(Renderer& renderer)
	: Buffer(renderer)
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

bool StructuredBuffer::createBuffer()
{
	UINT elementSize = 0;
	UINT numberOfElements = 0;

	return Buffer::createBuffer(
		D3D11_USAGE_DEFAULT,
		D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE,
		elementSize * numberOfElements,
		NULL,
		0,
		D3D11_RESOURCE_MISC_BUFFER_STRUCTURED,
		elementSize
	);
}
