#pragma once
#include "Buffer.h"
#include <d3d11.h>
#include "../Dev/Log.h"

//Denna ska ärva från buffer klassen. Men inte säker redigt hur ännu.

template<class T>
class VertexBuffer
{
private:
	ID3D11Buffer* buffer = nullptr;
	UINT stride = 0;
	UINT bufferSize = 0;
public:
	VertexBuffer();
	~VertexBuffer();

	ID3D11Buffer* getBuffer() const;
	UINT getStride() const;
	UINT getBufferSize() const;

	bool initialize(ID3D11Device* device, T* data, UINT numOfVertices);

};

template<class T>
inline VertexBuffer<T>::VertexBuffer()
{
}

template<class T>
inline VertexBuffer<T>::~VertexBuffer()
{
	if (this->buffer != nullptr)
	{
		buffer->Release();
	}
}

template<class T>
inline ID3D11Buffer* VertexBuffer<T>::getBuffer() const
{
	return this->buffer;
}

template<class T>
inline UINT VertexBuffer<T>::getStride() const
{
	return this->stride;
}

template<class T>
inline UINT VertexBuffer<T>::getBufferSize() const
{
	return this->bufferSize;
}

template<class T>
inline bool VertexBuffer<T>::initialize(ID3D11Device* device, T* data, UINT numOfVertices)
{
	if (this->buffer != nullptr)
	{
		buffer->Release();
	}
	
	this->bufferSize = numOfVertices;
	this->stride = sizeof(data);

	//Initialisera en simple buffer, tills vidare.
	
	D3D11_BUFFER_DESC desc = {};
	
	desc.ByteWidth = sizeof(T) * numOfVertices;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
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

	return true;
}
