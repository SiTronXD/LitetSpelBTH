#pragma once

#include "Buffer.h"

class StructuredBuffer : public Buffer
{
public:
	StructuredBuffer(Renderer& renderer);
	virtual ~StructuredBuffer();

	void updateBuffer(void* bufferData);

	bool createBuffer();
};