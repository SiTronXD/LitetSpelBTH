#pragma once

#include "Buffer.h"
#include "SRV.h"
#include "UAV.h"

class StructuredBuffer : public Buffer
{
private:
	SRV srv;
	UAV uav;

public:
	StructuredBuffer(Renderer& renderer,
		const std::string& debugName);
	virtual ~StructuredBuffer();

	void updateBuffer(void* bufferData);

	bool createBuffer(UINT elementSize, UINT numElements, void* initialData);
	inline SRV& getSrv() { return this->srv; }
	inline UAV& getUav() { return this->uav; }
};