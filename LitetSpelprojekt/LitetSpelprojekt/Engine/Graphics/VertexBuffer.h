#pragma once

#include "Buffer.h"
#include "MeshData.h"

class VertexBuffer : public Buffer
{
private:
	UINT stride;
	UINT offset;

public:
	VertexBuffer(Renderer& renderer);
	~VertexBuffer();

	bool createBuffer(MeshData& meshData);

	inline const UINT& getStride() { return this->stride; }
	inline const UINT& getOffset() { return this->offset; }
};