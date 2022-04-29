#pragma once

#include "Buffer.h"
#include "SRV.h"

class StructuredBuffer : public Buffer
{
private:
	SRV srv;

public:
	StructuredBuffer(Renderer& renderer,
		const std::string& debugName);
	virtual ~StructuredBuffer();

	void updateBuffer(void* bufferData);

	bool createBuffer(UINT elementSize, UINT numElements, void* initialData);
	inline SRV& getSrv() { return this->srv; }
};