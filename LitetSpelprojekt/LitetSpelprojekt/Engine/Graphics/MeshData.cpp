#include "MeshData.h"
#include "../Dev/Log.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

void MeshData::createTriangle()
{
	// Vertices
	this->vertices.push_back(this->makeVert(1.0f, 0.0f, 0.0f, 0.0f, 1.0f));
	this->vertices.push_back(this->makeVert(0.0f, 1.0f, 0.0f, 0.5f, 0.0f));
	this->vertices.push_back(this->makeVert(-1.0f, 0.0f, 0.0f, 1.0f, 1.0f));

	// Indices
	for (int i = 0; i < 3; ++i)
		this->indices.push_back(i);
}

void MeshData::createTetrahedron()
{
	// Vertices
	this->vertices.push_back(this->makeVert(-0.866f, 0.0f, 0.5f, 0.0f, 1.0f));
	this->vertices.push_back(this->makeVert(0.0f, 0.0f, -1.0f, 0.5f, 0.0f));
	this->vertices.push_back(this->makeVert(0.866f, 0.0f, 0.5f, 1.0f, 1.0f));

	this->vertices.push_back(this->makeVert(0.0f, 0.0f, -1.0f, 1.0f, 1.0f));
	this->vertices.push_back(this->makeVert(-0.866f, 0.0f, 0.5f, 0.0f, 1.0f));
	this->vertices.push_back(this->makeVert(0.0f, 1.5f, 0.0f, 0.5f, 0.0f));

	this->vertices.push_back(this->makeVert(0.866f, 0.0f, 0.5f, 1.0f, 1.0f));
	this->vertices.push_back(this->makeVert(0.0f, 0.0f, -1.0f, 0.0f, 1.0f));
	this->vertices.push_back(this->makeVert(0.0f, 1.5f, 0.0f, 0.5f, 0.0f));

	this->vertices.push_back(this->makeVert(-0.866f, 0.0f, 0.5f, 1.0f, 1.0f));
	this->vertices.push_back(this->makeVert(0.866f, 0.0f, 0.5f, 0.0f, 1.0f));
	this->vertices.push_back(this->makeVert(0.0f, 1.5f, 0.0f, 0.5f, 0.0f));


	// Indices
	for (int i = 0; i < 12; ++i)
		this->indices.push_back(i);
}

void MeshData::createCube()
{
	// Vertices

	// Front
	this->vertices.push_back(this->makeVert(0.5f, -0.5f, 0.5f, 0.0f, 1.0f));
	this->vertices.push_back(this->makeVert(0.5f, 0.5f, 0.5f, 0.0f, 0.0f));
	this->vertices.push_back(this->makeVert(-0.5f, -0.5f, 0.5f, 1.0f, 1.0f));
	this->vertices.push_back(this->makeVert(-0.5f, 0.5f, 0.5f, 1.0f, 0.0f));

	// Back
	this->vertices.push_back(this->makeVert(0.5f, -0.5f, -0.5f, 1.0f, 1.0f));
	this->vertices.push_back(this->makeVert(-0.5f, -0.5f, -0.5f, 0.0f, 1.0f));
	this->vertices.push_back(this->makeVert(0.5f, 0.5f, -0.5f, 1.0f, 0.0f));
	this->vertices.push_back(this->makeVert(-0.5f, 0.5f, -0.5f, 0.0f, 0.0f));

	// Top
	this->vertices.push_back(this->makeVert(0.5f, 0.5f, -0.5f, 1.0f, 1.0f));
	this->vertices.push_back(this->makeVert(-0.5f, 0.5f, -0.5f, 0.0f, 1.0f));
	this->vertices.push_back(this->makeVert(0.5f, 0.5f, 0.5f, 1.0f, 0.0f));
	this->vertices.push_back(this->makeVert(-0.5f, 0.5f, 0.5f, 0.0f, 0.0f));

	// Bottom
	this->vertices.push_back(this->makeVert(0.5f, -0.5f, -0.5f, 0.0f, 1.0f));
	this->vertices.push_back(this->makeVert(0.5f, -0.5f, 0.5f, 0.0f, 0.0f));
	this->vertices.push_back(this->makeVert(-0.5f, -0.5f, -0.5f, 1.0f, 1.0f));
	this->vertices.push_back(this->makeVert(-0.5f, -0.5f, 0.5f, 1.0f, 0.0f));

	// Left
	this->vertices.push_back(this->makeVert(0.5f, 0.5f, -0.5f, 0.0f, 0.0f));
	this->vertices.push_back(this->makeVert(0.5f, 0.5f, 0.5f, 1.0f, 0.0f));
	this->vertices.push_back(this->makeVert(0.5f, -0.5f, -0.5f, 0.0f, 1.0f));
	this->vertices.push_back(this->makeVert(0.5f, -0.5f, 0.5f, 1.0f, 1.0f));

	// Right
	this->vertices.push_back(this->makeVert(-0.5f, 0.5f, -0.5f, 1.0f, 0.0f));
	this->vertices.push_back(this->makeVert(-0.5f, -0.5f, -0.5f, 1.0f, 1.0f));
	this->vertices.push_back(this->makeVert(-0.5f, 0.5f, 0.5f, 0.0f, 0.0f));
	this->vertices.push_back(this->makeVert(-0.5f, -0.5f, 0.5f, 0.0f, 1.0f));

	// Indices
	for (int i = 0; i < 6; ++i)
	{
		// Triangle 1
		this->indices.push_back(i * 4 + 0);
		this->indices.push_back(i * 4 + 1);
		this->indices.push_back(i * 4 + 2);

		// Triangle 2
		this->indices.push_back(i * 4 + 1);
		this->indices.push_back(i * 4 + 3);
		this->indices.push_back(i * 4 + 2);
	}
}

void MeshData::createPlane(int resX, int resY)
{
	// Default resolution
	if (resX <= 1 || resY <= 1)
	{
		resX = 10;
		resY = 10;
	}

	// Vertices
	for (int y = 0; y < resY; ++y)
	{
		for (int x = 0; x < resX; ++x)
		{
			this->vertices.push_back(
				this->makeVert(
					((float)x / (resX - 1)) - 0.5f,
					0,
					((float)-y / (resY - 1)) + 0.5f,

					(float)x / (resX - 1),
					(float)y / (resY - 1)
				)
			);
		}
	}

	// Indices
	for (int i = 0; i < (resX - 1) * (resY - 1); ++i)
	{
		int squareX = i % (resX - 1);
		int squareY = i / (resX - 1);

		this->indices.push_back(squareY * resX + squareX + 0);
		this->indices.push_back(squareY * resX + squareX + resX + 1);
		this->indices.push_back(squareY * resX + squareX + resX);

		this->indices.push_back(squareY * resX + squareX + 0);
		this->indices.push_back(squareY * resX + squareX + 1);
		this->indices.push_back(squareY * resX + squareX + resX + 1);
	}
}

void MeshData::createSphere(int resX, int resY)
{
	// Default resolution
	if (resX <= 2 || resY <= 2)
	{
		resX = 10;
		resY = 10;
	}

	const float PI = 3.1415f;

	// Vertices
	for (int y = 0; y < resY; ++y)
	{
		double ty = (double)y / (resY - 1);

		for (int x = 0; x < resX; ++x)
		{
			double tx = (double)x / (resX - 1);

			float vx = (float)(sin(tx * 2.0 * PI) * sin(ty * PI));
			float vy = (float)-cos(ty * PI);
			float vz = (float)(cos(tx * 2.0 * PI) * sin(ty * PI));

			float vu = 1.0f - (float)tx;
			float vv = 1.0f - (float)ty;

			this->vertices.push_back(
				this->makeVert(
					vx, vy, vz,
					vu,
					vv
				)
			);
		}
	}

	// Indices
	for (int i = 0; i < (resX - 1) * (resY - 1); ++i)
	{
		int squareX = i % (resX - 1);
		int squareY = i / (resX - 1);

		this->indices.push_back(squareY * resX + squareX + 0);
		this->indices.push_back(squareY * resX + squareX + resX + 1);
		this->indices.push_back(squareY * resX + squareX + resX);

		this->indices.push_back(squareY * resX + squareX + 0);
		this->indices.push_back(squareY * resX + squareX + 1);
		this->indices.push_back(squareY * resX + squareX + resX + 1);
	}

	// Create triangles between the 2 vertical edges to avoid 
	// precision errors and z-fighting
	for (int i = 0; i < resY - 1; ++i)
	{
		int squareY = i * resX;

		this->indices.push_back(squareY + resX - 1 + 0);
		this->indices.push_back(squareY + resX - 1 + 1);
		this->indices.push_back(squareY + resX - 1 + resX);

		this->indices.push_back(squareY + resX - 1 + 0);
		this->indices.push_back(squareY + 0);
		this->indices.push_back(squareY + resX + 0);
	}
}

void MeshData::createLowPolyCylinder()
{
	// Vertices
	this->vertices.push_back(this->makeVert(-0.866f, 0.0f, 0.5f, 0.0f, 1.0f));
	this->vertices.push_back(this->makeVert(0.0f, 0.0f, -1.0f, 0.5f, 0.0f));
	this->vertices.push_back(this->makeVert(0.866f, 0.0f, 0.5f, 1.0f, 1.0f));

	this->vertices.push_back(this->makeVert(-0.866f, 1.0f, 0.5f, 0.0f, 1.0f));
	this->vertices.push_back(this->makeVert(0.0f, 1.0f, -1.0f, 0.5f, 0.0f));
	this->vertices.push_back(this->makeVert(0.866f, 1.0f, 0.5f, 1.0f, 1.0f));

	// Indices
	this->indices.push_back(0);
	this->indices.push_back(1);
	this->indices.push_back(2);

	this->indices.push_back(3);
	this->indices.push_back(5);
	this->indices.push_back(4);

	// Side 1
	this->indices.push_back(0);
	this->indices.push_back(3);
	this->indices.push_back(1);
	this->indices.push_back(1);
	this->indices.push_back(3);
	this->indices.push_back(4);

	// Side 2
	this->indices.push_back(1);
	this->indices.push_back(4);
	this->indices.push_back(2);
	this->indices.push_back(2);
	this->indices.push_back(4);
	this->indices.push_back(5);

	// Side 3
	this->indices.push_back(2);
	this->indices.push_back(5);
	this->indices.push_back(0);
	this->indices.push_back(0);
	this->indices.push_back(5);
	this->indices.push_back(3);
}

void MeshData::transformVector(
	const DirectX::SimpleMath::Matrix& transform,
	DirectX::XMFLOAT3& vec,
	bool isPosition)
{
	Vector4 vectorToTransform = Vector4(vec.x, vec.y, vec.z, isPosition ? 1.0f : 0.0f);

	// Position vectors
	if (isPosition)
	{
		vectorToTransform = XMVector4Transform(vectorToTransform, transform);
	}
	// Direction vectors
	else
	{
		transform.Invert();
		transform.Transpose();
		vectorToTransform = XMVector4Transform(vectorToTransform, transform);
		vectorToTransform = XMVector3Normalize(vectorToTransform);
	}

	// Store transformed vector
	XMStoreFloat3(&vec, vectorToTransform);
}

Vertex MeshData::makeVert(float xPos, float yPos, float zPos, float uTex, float vTex)
{
	return {
		{ xPos, yPos, zPos },		// Position		 
		{ 0.0, 0.0, -1.0 },			// Normal			 
		{ uTex, vTex }				// UVs
	};
}

MeshData::MeshData()
{ }

MeshData::MeshData(DefaultMesh defaultMesh)
{
	this->createDefault(defaultMesh);
}

MeshData::~MeshData()
{
}

void MeshData::createDefault(DefaultMesh defaultMesh)
{
	// Create vertex information
	switch (defaultMesh)
	{
	case DefaultMesh::TRIANGLE:

		this->createTriangle();

		break;

	case DefaultMesh::TETRAHEDRON:

		this->createTetrahedron();

		break;

	case DefaultMesh::CUBE:

		this->createCube();

		break;

	case DefaultMesh::PLANE:

		this->createPlane();

		break;

	case DefaultMesh::SPHERE:

		this->createSphere();

		break;

	case DefaultMesh::LOW_POLY_CYLINDER:

		this->createLowPolyCylinder();

		break;
	}

	// One single submesh
	Submesh newSubmesh{};
	newSubmesh.numIndices = this->indices.size();
	this->submeshes.clear();
	this->submeshes.push_back(newSubmesh);

	this->invertFaces();
	this->calculateNormals(defaultMesh);
}

void MeshData::calculateNormals()
{
	if (!this->hasAnimations())
	{
		// Choose anything that's not a sphere
		this->calculateNormals(DefaultMesh::TRIANGLE);
	}
	else
	{
		// Choose anything that's not a sphere
		this->calculateAnimNormals(DefaultMesh::TRIANGLE);
	}
}

void MeshData::calculateNormals(DefaultMesh defaultMesh)
{
	if (defaultMesh != DefaultMesh::SPHERE)
	{
		std::vector<XMFLOAT3> vertexNormals;
		vertexNormals.resize(this->vertices.size(), { 0.0f, 0.0f, 0.0f });

		// Add flat normals to each vertex
		for (unsigned int i = 0; i < this->indices.size(); i += 3)
		{
			XMVECTOR vertPos0 = XMLoadFloat3(&this->vertices[this->indices[i + 0]].pos);
			XMVECTOR vertPos1 = XMLoadFloat3(&this->vertices[this->indices[i + 1]].pos);
			XMVECTOR vertPos2 = XMLoadFloat3(&this->vertices[this->indices[i + 2]].pos);

			XMVECTOR edge1 = vertPos1 - vertPos0;
			XMVECTOR edge2 = vertPos2 - vertPos0;

			XMVECTOR normal = XMVector3Normalize(XMVector3Cross(edge2, edge1));

			// Add calculated normal to current list of vertex normals
			XMStoreFloat3(
				&vertexNormals[this->indices[i + 0]],
				XMLoadFloat3(&vertexNormals[this->indices[i + 0]]) + normal
			);
			XMStoreFloat3(
				&vertexNormals[this->indices[i + 1]],
				XMLoadFloat3(&vertexNormals[this->indices[i + 1]]) + normal
			);
			XMStoreFloat3(
				&vertexNormals[this->indices[i + 2]],
				XMLoadFloat3(&vertexNormals[this->indices[i + 2]]) + normal
			);
		}

		// Normalize vector sums
		for (unsigned int i = 0; i < this->vertices.size(); ++i)
		{
			XMVECTOR vec = XMLoadFloat3(&vertexNormals[i]);

			// Normalize, if possible
			XMFLOAT3 dotResult;
			XMStoreFloat3(&dotResult, XMVector3Dot(vec, vec));
			if (dotResult.x > 0.0f)
				vec = XMVector3Normalize(vec);

			// Store normal
			XMFLOAT3 finalNormal;
			XMStoreFloat3(&finalNormal, vec);
			this->vertices[i].normal = finalNormal;
		}
	}
	// Sphere normals
	else
	{
		for (unsigned int i = 0; i < this->vertices.size(); ++i)
		{
			XMVECTOR vec = XMLoadFloat3(&this->vertices[i].pos);

			// Normalize position
			vec = XMVector3Normalize(vec);

			// Store normal
			XMFLOAT3 finalNormal;
			XMStoreFloat3(&finalNormal, vec);
			this->vertices[i].normal = finalNormal;
		}
	}
}

void MeshData::calculateAnimNormals(DefaultMesh defaultMesh)
{
	if (defaultMesh != DefaultMesh::SPHERE)
	{
		std::vector<XMFLOAT3> vertexNormals;
		vertexNormals.resize(this->animVertices.size(), { 0.0f, 0.0f, 0.0f });

		// Add flat normals to each vertex
		for (unsigned int i = 0; i < this->indices.size(); i += 3)
		{
			XMVECTOR vertPos0 = XMLoadFloat3(&this->animVertices[this->indices[i + 0]].pos);
			XMVECTOR vertPos1 = XMLoadFloat3(&this->animVertices[this->indices[i + 1]].pos);
			XMVECTOR vertPos2 = XMLoadFloat3(&this->animVertices[this->indices[i + 2]].pos);

			XMVECTOR edge1 = vertPos1 - vertPos0;
			XMVECTOR edge2 = vertPos2 - vertPos0;

			XMVECTOR normal = XMVector3Normalize(XMVector3Cross(edge2, edge1));

			// Add calculated normal to current list of vertex normals
			XMStoreFloat3(
				&vertexNormals[this->indices[i + 0]],
				XMLoadFloat3(&vertexNormals[this->indices[i + 0]]) + normal
			);
			XMStoreFloat3(
				&vertexNormals[this->indices[i + 1]],
				XMLoadFloat3(&vertexNormals[this->indices[i + 1]]) + normal
			);
			XMStoreFloat3(
				&vertexNormals[this->indices[i + 2]],
				XMLoadFloat3(&vertexNormals[this->indices[i + 2]]) + normal
			);
		}

		// Normalize vector sums
		for (unsigned int i = 0; i < this->animVertices.size(); ++i)
		{
			XMVECTOR vec = XMLoadFloat3(&vertexNormals[i]);

			// Normalize, if possible
			XMFLOAT3 dotResult;
			XMStoreFloat3(&dotResult, XMVector3Dot(vec, vec));
			if (dotResult.x > 0.0f)
				vec = XMVector3Normalize(vec);

			// Store normal
			XMFLOAT3 finalNormal;
			XMStoreFloat3(&finalNormal, vec);
			this->animVertices[i].normal = finalNormal;
		}
	}
	// Sphere normals
	else
	{
		for (unsigned int i = 0; i < this->animVertices.size(); ++i)
		{
			XMVECTOR vec = XMLoadFloat3(&this->animVertices[i].pos);

			// Normalize position
			vec = XMVector3Normalize(vec);

			// Store normal
			XMFLOAT3 finalNormal;
			XMStoreFloat3(&finalNormal, vec);
			this->animVertices[i].normal = finalNormal;
		}
	}
}

void MeshData::invertFaces()
{
	// Swap 2 indices in each triangle
	for (size_t i = 0; i < this->indices.size(); i += 3)
	{
		int temp = this->indices[i];
		this->indices[i] = this->indices[i + 1];
		this->indices[i + 1] = temp;
	}
}

void MeshData::transformMesh(const DirectX::SimpleMath::Matrix& transform)
{
	if (!this->hasAnimations())
	{
		for (unsigned int i = 0; i < this->vertices.size(); ++i)
		{
			this->transformVector(transform, this->vertices[i].pos, true);
			this->transformVector(transform, this->vertices[i].normal, false);
		}
	}
	else
	{
		for (unsigned int i = 0; i < this->animVertices.size(); ++i)
		{
			this->transformVector(transform, this->animVertices[i].pos, true);
			this->transformVector(transform, this->animVertices[i].normal, false);
		}
	}
}

void MeshData::addVertex(const Vertex& newVertex)
{
	this->vertices.push_back(newVertex);
}

void MeshData::addAnimVertex(const AnimVertex& newAnimVertex)
{
	this->animVertices.push_back(newAnimVertex);
}

void MeshData::addIndex(const unsigned int& newIndex)
{
	this->indices.push_back(newIndex);
}

void MeshData::addSubmesh(const Submesh& newSubmesh)
{
	this->submeshes.push_back(newSubmesh);
}

void MeshData::addBone(const Bone& bone)
{
	this->skeleton.push_back(bone);
}

void MeshData::swapBones(Bone& bone, const unsigned int& destinationIndex, const unsigned int& sourceIndex)
{
	if (destinationIndex != sourceIndex)
	{
		Bone tempBone = this->skeleton[destinationIndex];
		this->skeleton[destinationIndex] = bone;
		this->skeleton[sourceIndex] = tempBone;
	}
}

unsigned int MeshData::getBoneIndex(
	const std::string& boneName, Bone*& outputBone)
{
	for (unsigned int i = 0; i < this->skeleton.size(); ++i)
	{
		// Found bone
		if (this->skeleton[i].name == boneName)
		{
			outputBone = &this->skeleton[i];
			return i;
		}
	}

	Log::error("Could not find bone with the name " + boneName);
	return -1;
}
