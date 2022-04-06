#pragma once

#include <vector>
#include <DirectXMath.h>

struct Vertex
{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 normal;
	DirectX::XMFLOAT2 uv;
};

class MeshData
{
private:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

public:
	MeshData();
	virtual ~MeshData();

	inline const std::vector<Vertex>& getVertices() { return this->vertices; }
	inline const std::vector<unsigned int>& getIndices() { return this->indices; }
};