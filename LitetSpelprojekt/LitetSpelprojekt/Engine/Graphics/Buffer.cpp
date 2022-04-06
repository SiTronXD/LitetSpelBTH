#include "../Dev/Log.h"
#include "../Dev/Helpers.h"
#include "Buffer.h"
#include "Renderer.h"

Buffer::Buffer(Renderer& renderer) 
	: renderer(renderer),
	buffer(nullptr),
	bufferSize(-1)
{ }

Buffer::~Buffer()
{ 
	S_RELEASE(this->buffer);
}

bool Buffer::createBuffer(
	D3D11_USAGE usage, UINT bindFlags, UINT bufferSize,
	void* initialData,
	UINT cpuAccessFlags,
	UINT miscFlags,
	UINT structureByteStride)
{
	// Remove old buffer
	S_RELEASE(this->buffer);

	this->bufferSize = bufferSize;

	// Buffer description
	D3D11_BUFFER_DESC bufferDesc{};
	bufferDesc.ByteWidth = bufferSize;
	bufferDesc.Usage = usage;
	bufferDesc.BindFlags = bindFlags;

	bufferDesc.CPUAccessFlags = cpuAccessFlags;
	bufferDesc.MiscFlags = miscFlags;
	bufferDesc.StructureByteStride = structureByteStride;

	// Subresource data
	D3D11_SUBRESOURCE_DATA subData{};
	subData.pSysMem = initialData;
	subData.SysMemPitch = 0;
	subData.SysMemSlicePitch = 0;

	// Create buffer
	HRESULT hr = this->renderer.getDevice()->CreateBuffer(
		&bufferDesc, 
		initialData != NULL ? &subData : NULL,
		&this->buffer
	);
	if (FAILED(hr))
	{
		Log::error("Failed to create buffer.");
		return false;
	}

	return true;
}

void Buffer::map(D3D11_MAPPED_SUBRESOURCE& outputMappedSubresource)
{
	// Make sure buffer has been initialized
	if (this->buffer == nullptr)
	{
		Log::error(
			"This buffer has not been initialized and therefore cannot be mapped: "
		);

		return;
	}

	// Map constant buffer
	HRESULT hr = this->renderer.getDeviceContext()->Map(
		this->buffer,
		NULL,
		D3D11_MAP_WRITE_DISCARD,
		NULL,
		&outputMappedSubresource
	);

	if (FAILED(hr))
		Log::error("Failed to map buffer.");
}

void Buffer::unmap()
{
	this->renderer.getDeviceContext()->Unmap(this->buffer, 0);
}
