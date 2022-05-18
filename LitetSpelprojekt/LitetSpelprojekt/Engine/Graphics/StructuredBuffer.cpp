#include "StructuredBuffer.h"

StructuredBuffer::StructuredBuffer(Renderer& renderer, const std::string& debugName)
	: Buffer(renderer, debugName),
	srv(renderer, "Structured Buffer SRV"),
	uav(renderer, "Structured Buffer UAV")
{
}

StructuredBuffer::~StructuredBuffer()
{
}

void StructuredBuffer::cpuUpdateBuffer(void* bufferData)
{
	D3D11_MAPPED_SUBRESOURCE mappedSubresource;
	Buffer::map(mappedSubresource);

	// Copy over data
	memcpy(mappedSubresource.pData, bufferData, Buffer::getBufferSize());

	Buffer::unmap();
}

bool StructuredBuffer::createBuffer(
	UINT elementSize, UINT numElements,
	void* initialData, D3D11_USAGE usage, 
	UINT bindFlags, bool cpuWrite)
{
	bool success = Buffer::createBuffer(
		usage,
		bindFlags,
		elementSize * numElements,
		initialData,
		cpuWrite ? D3D11_CPU_ACCESS_WRITE : 0,
		D3D11_RESOURCE_MISC_BUFFER_STRUCTURED,
		elementSize
	);

	// SRV
	if ((bindFlags & D3D11_BIND_SHADER_RESOURCE) == D3D11_BIND_SHADER_RESOURCE)
	{
		success = this->srv.createBufferSRV(
			this->getBuffer(), DXGI_FORMAT_UNKNOWN, numElements
		);
	}

	// UAV
	if ((bindFlags & D3D11_BIND_UNORDERED_ACCESS) == D3D11_BIND_UNORDERED_ACCESS)
	{
		success = this->uav.createBufferUAV(this->getBuffer(), DXGI_FORMAT_UNKNOWN, numElements);
	}

	return success;
}
