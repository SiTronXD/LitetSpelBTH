#pragma once

#include "Buffer.h"

class ConstantBuffer : public Buffer
{
public:
	ConstantBuffer(Renderer& renderer, const std::string& debugName);
	virtual ~ConstantBuffer();

	void updateBuffer(void* bufferData);

	bool createBuffer(UINT bufferSize);
};