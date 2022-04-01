#pragma once
#include <d3d11.h>

template<class T>
class Buffer
{
private:
	ID3D11Buffer* buffer = nullptr;
public:
	virtual Buffer() = 0;
	virtual ~Buffer() = 0;

	virtual setBuffer(ID3D11Buffer& buffer) = 0;
	virtual ID3D11Buffer* getBuffer() const = 0;


};

