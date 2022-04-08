#pragma once

#include "Buffer.h"

class StructuredBuffer : public Buffer
{
public:
	StructuredBuffer(Renderer& renderer,
		const std::string& debugName);
	virtual ~StructuredBuffer();

	void updateBuffer(void* bufferData);

	bool createBuffer();
};