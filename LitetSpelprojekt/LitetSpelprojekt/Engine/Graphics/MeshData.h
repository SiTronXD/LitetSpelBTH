#pragma once

#include <vector>
#include <Windows.h>
#include <SimpleMath.h>

struct Vertex
{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 normal;
	DirectX::XMFLOAT2 uv;
};

enum class DefaultMesh
{
	TRIANGLE,
	TETRAHEDRON,
	CUBE,
	PLANE,
	SPHERE
};

class MeshData
{
private:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	void createTriangle();
	void createTetrahedron();
	void createCube();
	void createPlane(int resX = 10, int resY =  10);
	void createSphere(int resX = 10, int resY = 10);

	void transformVector(
		const DirectX::SimpleMath::Matrix& transform, 
		DirectX::XMFLOAT3& vec,
		bool isPosition
	);

	Vertex makeVert(float xPos, float yPos, float zPos, float uTex, float vTex);

public:
	MeshData();
	MeshData(DefaultMesh defaultMesh);
	virtual ~MeshData();

	void createDefault(DefaultMesh defaultMesh);
	void calculateNormals(DefaultMesh defaultMesh);
	void invertFaces();
	void transformMesh(const DirectX::SimpleMath::Matrix& transform);

	void addVertex(const Vertex& newVertex);
	void addIndex(const unsigned int& newIndex);

	inline const std::vector<Vertex>& getVertices() { return this->vertices; }
	inline const std::vector<unsigned int>& getIndices() { return this->indices; }
};