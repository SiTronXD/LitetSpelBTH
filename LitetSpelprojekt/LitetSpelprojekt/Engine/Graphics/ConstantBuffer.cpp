#include "ConstantBuffer.h"

ConstantBuffer::ConstantBuffer(Renderer& renderer)
	: Buffer(renderer)
{
}

ConstantBuffer::~ConstantBuffer()
{
}

void ConstantBuffer::updateBuffer(void* bufferData)
{
	D3D11_MAPPED_SUBRESOURCE mappedSubresource;
	Buffer::map(mappedSubresource);

	// Copy over data
	memcpy(mappedSubresource.pData, bufferData, Buffer::getBufferSize());

	Buffer::unmap();
}

bool ConstantBuffer::createBuffer(UINT bufferSize)
{
	return Buffer::createBuffer(
		D3D11_USAGE_DYNAMIC,
		D3D11_BIND_CONSTANT_BUFFER,
		bufferSize,
		NULL
	);
}
