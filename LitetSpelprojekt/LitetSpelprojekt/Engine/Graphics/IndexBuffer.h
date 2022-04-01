#pragma once
#include <d3d11.h>
#include "Buffer.h"
#include "../Dev/Log.h"

class IndexBuffer
{
private:
	ID3D11Buffer* buffer = nullptr;
	UINT bufferSize = 0;
public:
	IndexBuffer();
	~IndexBuffer()
	{
		if (this->buffer != nullptr)
		{
			this->buffer->Release();
		}
	}

	ID3D11Buffer* getBuffer() const
	{
		return this->buffer;
	}
	UINT getBufferSize() const
	{
		return this->bufferSize;
	}

	bool initialize(ID3D11Device* device, UINT* data, UINT numOfIndices)
	{
		if (this->buffer != nullptr)
		{
			this->buffer->Release();
		}

		this->bufferSize = numOfIndices;

		D3D11_BUFFER_DESC desc = {};

		desc.ByteWidth = sizeof(data) * numOfIndices;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;
		//desc.StructureByteStride = data;

		D3D11_SUBRESOURCE_DATA subData = {};
		subData.pSysMem = data;

		HRESULT hr = device->CreateBuffer(&desc, &subData, &this->buffer);

		if (FAILED(hr))
		{
			Log::error("Error creating vertex buffer!");
			return false;
		}
	}

};