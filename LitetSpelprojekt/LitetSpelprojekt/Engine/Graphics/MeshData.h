#pragma once

#include <string>
#include <vector>
#include <Windows.h>
#include <SimpleMath.h>

struct Vertex
{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 normal;
	DirectX::XMFLOAT2 uv;
};

struct AnimVertex
{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 normal;
	DirectX::XMFLOAT2 uv;

	DirectX::XMFLOAT4 boneWeights;
	DirectX::XMINT4 boneIndices;
};

struct BoneTransforms
{
	std::vector<std::pair<double, DirectX::XMFLOAT3>> positionStamps;
	std::vector<std::pair<double, DirectX::XMFLOAT4>> rotationStamps;
	std::vector<std::pair<double, DirectX::XMFLOAT3>> scaleStamps;
};

struct Bone
{
	std::string name;
	int parentIndex;

	BoneTransforms boneTransforms;

	// (vertex index, weight)
	std::vector<std::pair<unsigned int, double>> weights;

	DirectX::XMFLOAT4X4 bindModelInverse;
	DirectX::XMFLOAT4X4 modelTransform;
	DirectX::XMFLOAT4X4 finalTransform;
};

struct Submesh
{
	char materialName[64];

	unsigned int startIndex;
	unsigned int numIndices;
};

enum class DefaultMesh
{
	TRIANGLE,
	TETRAHEDRON,
	CUBE,
	PLANE,
	SPHERE,
	LOW_POLY_CYLINDER
};

class MeshData
{
private:
	std::vector<Vertex> vertices;
	std::vector<AnimVertex> animVertices;
	std::vector<unsigned int> indices;
	std::vector<Submesh> submeshes;
	std::vector<Bone> skeleton;

	void createTriangle();
	void createTetrahedron();
	void createCube();
	void createPlane(int resX = 2, int resY =  2);
	void createSphere(int resX = 10, int resY = 10);
	void createLowPolyCylinder();

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
	void calculateNormals();
	void calculateNormals(DefaultMesh defaultMesh);
	void calculateAnimNormals(DefaultMesh defaultMesh);
	void invertFaces();
	void transformMesh(const DirectX::SimpleMath::Matrix& transform);

	void addVertex(const Vertex& newVertex);
	void addAnimVertex(const AnimVertex& newAnimVertex);
	void addIndex(const unsigned int& newIndex);
	void addSubmesh(const Submesh& newSubmesh);
	void addBone(const Bone& bone);

	void swapBones(Bone& bone, const unsigned int& destinationIndex, const unsigned int& sourceIndex);

	unsigned int getBoneIndex(const std::string& boneName, Bone*& outputBone);

	inline std::vector<Vertex>& getVertices() { return this->vertices; }
	inline std::vector<AnimVertex>& getAnimVertices() { return this->animVertices; }
	inline std::vector<unsigned int>& getIndices() { return this->indices; }
	inline std::vector<Submesh>& getSubmeshes() { return this->submeshes; }
	inline std::vector<Bone>& getSkeleton() { return this->skeleton; }

	inline bool hasAnimations() { return this->animVertices.size() > 0; }
};