#pragma once

#include "Buffer.h"
#include "MeshData.h"

class IndexBuffer : public Buffer
{
private:
	UINT indexCount;

public:
	IndexBuffer(Renderer& renderer);
	virtual ~IndexBuffer();

	bool createBuffer(MeshData& meshData);

	inline const UINT& getIndexCount() { return this->indexCount; }
};