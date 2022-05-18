#include <iostream>
#include "LevelLoader.h"
#include "../../Engine/Dev/Log.h"
#include "../../Engine/Dev/Str.h"
#include "../../Engine/SMath.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

//#define LOG_NODE_NAMES

void LevelLoader::switchFloats(Vector3& vec)
{
	float t = vec.y;
	vec.y = vec.z;
	vec.z = t;
}

void LevelLoader::switchFloats(XMFLOAT3& vec)
{
	float t = vec.y;
	vec.y = vec.z;
	vec.z = t;
}

void LevelLoader::traverseStructure(
	aiNode* node,
	const DirectX::SimpleMath::Matrix& parentTransform)
{
	std::string nodeName = std::string(node->mName.C_Str());

#ifdef LOG_NODE_NAMES
	Log::write(nodeName);
#endif

	// Node transformation
	Matrix transformation = parentTransform;
	Matrix nodeTransform;
	SMath::assimpToMat(node->mTransformation, nodeTransform);
	transformation *= nodeTransform;

	// Decompose transformation
	Vector3 nodePosition;
	Vector4 nodeRotationQuaternion;
	Vector3 nodeScale;
	XMVECTOR tempNodePosition;
	XMVECTOR tempNodeRotationQuaternion;
	XMVECTOR tempNodeScale;
	XMMatrixDecompose(
		&tempNodeScale,
		&tempNodeRotationQuaternion,
		&tempNodePosition,
		transformation
	);
	nodePosition = tempNodePosition;
	nodeRotationQuaternion = tempNodeRotationQuaternion;
	nodeScale = tempNodeScale;

	// Colliders
	if (nodeName.find("collider_") != std::string::npos)
	{
		// Sphere
		if (nodeName.find("sphere") != std::string::npos)
		{
			// Create and add sphere collider
			LevelColliderSphere newColliderSphere{};
			newColliderSphere.pos = nodePosition;
			newColliderSphere.radius = nodeScale.x;
			this->sphereColliders.push_back(newColliderSphere);
		}
		// Box
		else if (nodeName.find("cube") != std::string::npos)
		{
			// Create and add box collider
			LevelColliderBox newColliderBox{};
			newColliderBox.pos = nodePosition;
			newColliderBox.extents = nodeScale;
			this->switchFloats(newColliderBox.extents);
			this->boxColliders.push_back(newColliderBox);
		}
		// Oriented box
		else if (nodeName.find("orientedBox") != std::string::npos)
		{
			// Create and add oriented box collider
			LevelColliderOrientedBox newColliderOrientedBox{};
			newColliderOrientedBox.pos = nodePosition;
			newColliderOrientedBox.extents = nodeScale;
			newColliderOrientedBox.orientation = nodeRotationQuaternion;
			this->switchFloats(newColliderOrientedBox.extents);
			this->orientedBoxColliders.push_back(newColliderOrientedBox);
		}
	}
	// Player start position
	else if (nodeName == "PlayerStartPos")
	{
		this->playerStartPos = nodePosition;
	}
	// Keys
	else if (nodeName.find("interactable_key") != std::string::npos)
	{
		// Get key index
		nodeName.erase(0, 16);
		unsigned int keyIndex = std::stoi(nodeName);

		// Set key position
		if (keyIndex < this->NUM_KEYS)
		{
			this->keys[keyIndex].position = nodePosition;
		}
	}
	// Portal
	else if (nodeName == "interactable_portal")
	{
		this->portal.position = nodePosition;
		this->portal.scale = nodeScale * 2.0f;
		this->switchFloats(this->portal.scale);
	}
	// Spikes
	else if (nodeName.find("interactable_spike") != std::string::npos)
	{
		SpikeInfo newSpikeInfo{};
		newSpikeInfo.position = nodePosition;
		newSpikeInfo.rotation = nodeRotationQuaternion;
		newSpikeInfo.scale = nodeScale;

		this->spikes.push_back(newSpikeInfo);
	}
	// Mesh
	else
	{

		// Add meshes for this node
		for (int i = 0; i < node->mNumMeshes; ++i)
		{
			// Add mesh
			this->addMeshToMegaMesh(
				*this->allMeshes[node->mMeshes[i]],
				transformation
			);
		}

		// Traverse through child node
		for (int i = 0; i < node->mNumChildren; ++i)
			this->traverseStructure(node->mChildren[i], transformation);
	}
}

void LevelLoader::addMeshToMegaMesh(
	MeshData meshData,
	const DirectX::SimpleMath::Matrix transformation)
{
	unsigned int initialVertexCount = this->megaMesh.getVertices().size();
	unsigned int initialIndexCount = this->megaMesh.getIndices().size();

	// Transform
	meshData.transformMesh(transformation);

	// Vertices
	for (unsigned int i = 0; i < meshData.getVertices().size(); ++i)
	{
		this->megaMesh.addVertex(
			meshData.getVertices()[i]
		);
	}

	// Indices
	for (unsigned int i = 0; i < meshData.getIndices().size(); ++i)
	{
		this->megaMesh.addIndex(
			initialVertexCount +
			meshData.getIndices()[i]
		);
	}

	// Submeshes
	for (unsigned int i = 0; i < meshData.getSubmeshes().size(); ++i)
	{
		Submesh newSubmesh{};
		strcpy_s(
			newSubmesh.materialName,
			meshData.getSubmeshes()[i].materialName
		);
		newSubmesh.startIndex = initialIndexCount +
			meshData.getSubmeshes()[i].startIndex;
		newSubmesh.numIndices = meshData.getSubmeshes()[i].numIndices;

		this->megaMesh.addSubmesh(newSubmesh);
	}
}

DirectX::SimpleMath::Vector3 LevelLoader::getAveragePosition(aiMesh* submesh)
{
	// Average position
	Vector3 avgPos = Vector3(0, 0, 0);
	for (unsigned int j = 0; j < submesh->mNumVertices; ++j)
	{
		avgPos += Vector3(
			submesh->mVertices[j].x,
			submesh->mVertices[j].y,
			submesh->mVertices[j].z
		);
	}
	avgPos /= (float)submesh->mNumVertices;

	return avgPos;
}

LevelLoader::LevelLoader(Resources& resources)
	: resources(resources),
	playerStartPos(0,0,0)
{
	this->keys.resize(this->NUM_KEYS);

	// Colors
	this->keys[0].color = Vector3(1.0f, 1.0f, 0.0f);
	this->keys[1].color = Vector3(0.0f, 1.0f, 0.0f);
	this->keys[2].color = Vector3(0.0f, 0.0f, 1.0f);
	this->keys[3].color = Vector3(1.0f, 0.0f, 1.0f);
}

LevelLoader::~LevelLoader()
{
}

bool LevelLoader::load(const std::string& levelName)
{
	bool flipVerticalUV = true;

	// Load
	const aiScene* scene = aiImportFile(levelName.c_str(), aiProcessPreset_TargetRealtime_MaxQuality);
	if (!scene)
	{
		Log::error("Could not load level: " + std::string(aiGetErrorString()));
		return false;
	}

	// Materials
	std::vector<std::string> diffuseTexturePaths(scene->mNumMaterials);
	for (int i = 0; i < scene->mNumMaterials; ++i)
	{
		// Get diffuse texture
		aiString path;
		if (scene->mMaterials[i]->GetTexture(
			aiTextureType_DIFFUSE,
			0,
			&path,
			NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
		{
			// Texture path
			diffuseTexturePaths[i] = std::string(path.C_Str());

			// Replace .tga with .png
			if (diffuseTexturePaths[i].find(".tga") != std::string::npos)
			{
				diffuseTexturePaths[i].erase(
					diffuseTexturePaths[i].size() - 3,
					3
				);
				diffuseTexturePaths[i] += "png";
			}

			// Pick the last texture name of either absolute/relative path
			std::vector<std::string> splitPath;
			Str::splitString(diffuseTexturePaths[i], '\\', splitPath);
			diffuseTexturePaths[i] = splitPath[splitPath.size() - 1];

			// Add texture and material
			this->resources.addTexture(
				"Resources/Textures/" + diffuseTexturePaths[i],
				diffuseTexturePaths[i]
			);
			this->resources.addMaterial(
				diffuseTexturePaths[i],
				diffuseTexturePaths[i]
			);
		}
	}

	// Loop through each submesh
	for (unsigned int i = 0; i < scene->mNumMeshes; ++i)
	{
		aiMesh* submesh = scene->mMeshes[i];

		MeshData* newMeshData = new MeshData();

		// Loop through each vertex
		for (unsigned int j = 0; j < submesh->mNumVertices; ++j)
		{
			// Load data
			aiVector3D pos = submesh->mVertices[j];
			aiVector3D norm = submesh->mNormals[j];
			aiVector3D uv = submesh->mTextureCoords[0][j];

			if (flipVerticalUV)
				uv.y = 1.0f - uv.y;

			// Create vertex with data
			Vertex newVertex{};
			newVertex.pos = XMFLOAT3(pos.x, pos.y, pos.z);
			newVertex.normal = XMFLOAT3(norm.x, norm.y, norm.z);
			newVertex.uv = XMFLOAT2(uv.x, uv.y);

			newMeshData->addVertex(newVertex);
		}

		// Loop through each index
		unsigned int numIndices = 0;
		unsigned int indicesSize = this->megaMesh.getIndices().size();
		for (unsigned int j = 0; j < submesh->mNumFaces; ++j)
		{
			if (submesh->mFaces[j].mNumIndices != 3u)
				continue;

			newMeshData->addIndex(submesh->mFaces[j].mIndices[0]);
			newMeshData->addIndex(submesh->mFaces[j].mIndices[1]);
			newMeshData->addIndex(submesh->mFaces[j].mIndices[2]);
			
			numIndices += 3;
		}

		Submesh newSubmesh{};
		newSubmesh.startIndex = indicesSize;
		newSubmesh.numIndices = numIndices;
		strcpy_s(newSubmesh.materialName, diffuseTexturePaths[submesh->mMaterialIndex].c_str());
		newMeshData->addSubmesh(newSubmesh);

		this->allMeshes.push_back(newMeshData);
	}

	// Place all meshes in mega mesh
	this->traverseStructure(scene->mRootNode, Matrix());

	// Deallocate all old meshes
	for (MeshData* meshData : allMeshes)
		delete meshData;

	aiReleaseImport(scene);

	this->megaMesh.invertFaces();
	this->megaMesh.calculateNormals();

	return true;
}
