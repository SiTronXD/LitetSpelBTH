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

	void cpuUpdateBuffer(void* bufferData);

	bool createBuffer(UINT elementSize, 
		UINT numElements, 
		void* initialData, 
		D3D11_USAGE usage = D3D11_USAGE_DEFAULT,
		UINT bindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE,
		bool cpuWrite = false);
	inline SRV& getSrv() { return this->srv; }
	inline UAV& getUav() { return this->uav; }
};