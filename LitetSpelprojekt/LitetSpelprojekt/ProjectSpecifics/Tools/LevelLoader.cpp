#include "LevelLoader.h"
#include "../../Engine/Dev/Log.h"

#include <DirectXMath.h>

using namespace DirectX;
using namespace DirectX::SimpleMath;

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
	unsigned int vertexOffset = 0;
	for (unsigned int i = 0; i < scene->mNumMeshes; ++i)
	{
		aiMesh* submesh = scene->mMeshes[i];

		std::string submeshName = std::string(submesh->mName.C_Str());

		// Colliders
		if (submeshName.find("collider_") != std::string::npos)
		{
			Vector3 avgPos = this->getAveragePosition(submesh);

			// Sphere
			if (submeshName.find("sphere") != std::string::npos)
			{
				// First position in sphere surface
				Vector3 surfacePos = Vector3(
					submesh->mVertices[0].x,
					submesh->mVertices[0].y,
					submesh->mVertices[0].z
				);

				// Create and add sphere collider
				LevelColliderSphere newColliderSphere{};
				newColliderSphere.pos = avgPos;
				newColliderSphere.radius = (avgPos - surfacePos).Length();
				this->sphereColliders.push_back(newColliderSphere);
			}
			// Box
			else if (submeshName.find("cube") != std::string::npos)
			{
				Vector3 extents = Vector3(
					abs(avgPos.x - submesh->mVertices[0].x),
					abs(avgPos.y - submesh->mVertices[0].y),
					abs(avgPos.z - submesh->mVertices[0].z)
				);

				// Create and add box collider
				LevelColliderBox newColliderBox{};
				newColliderBox.pos = avgPos;
				newColliderBox.extents = extents;
				this->boxColliders.push_back(newColliderBox);
			}
			// Oriented box
			else if (submeshName.find("orientedBox") != std::string::npos)
			{
				// Create XMFLOAT3 array
				XMFLOAT3* points = new XMFLOAT3[submesh->mNumVertices]{};
				for (int j = 0; j < submesh->mNumVertices; ++j)
				{
					points[j] = XMFLOAT3(
						submesh->mVertices[j].x,
						submesh->mVertices[j].y,
						submesh->mVertices[j].z
					);
				}

				// Create oriented box
				BoundingOrientedBox tempBox;
				BoundingOrientedBox::CreateFromPoints(
					tempBox, 
					submesh->mNumVertices, 
					points,
					sizeof(XMFLOAT3)
				);
				delete[] points;

				// Create and add oriented box collider
				LevelColliderOrientedBox newColliderOrientedBox{};
				newColliderOrientedBox.pos = avgPos;
				newColliderOrientedBox.extents = tempBox.Extents;
				// newColliderOrientedBox.orientation = eulerangle tempBox.Orientation;
				this->orientedBoxColliders.push_back(newColliderOrientedBox);
			}
		}
		// Player start position
		else if (submeshName == "PlayerStartPos")
		{
			this->playerStartPos = this->getAveragePosition(submesh);
		}
		// Mesh
		else
		{
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

				this->meshData.addVertex(newVertex);
			}

			// Loop through each index
			unsigned int numIndices = 0;
			unsigned int indicesSize = this->meshData.getIndices().size();
			for (unsigned int j = 0; j < submesh->mNumFaces; ++j)
			{
				if (submesh->mFaces[j].mNumIndices != 3u)
					continue;

				this->meshData.addIndex(vertexOffset + submesh->mFaces[j].mIndices[0]);
				this->meshData.addIndex(vertexOffset + submesh->mFaces[j].mIndices[1]);
				this->meshData.addIndex(vertexOffset + submesh->mFaces[j].mIndices[2]);
			
				numIndices += 3;
			}

			Submesh newSubmesh{};
			newSubmesh.startIndex = indicesSize;
			newSubmesh.numIndices = numIndices;
			strcpy_s(newSubmesh.materialName, diffuseTexturePaths[submesh->mMaterialIndex].c_str());
			this->meshData.addSubmesh(newSubmesh);

			// Update offset for next submesh
			vertexOffset = this->meshData.getVertices().size();
		}
	}

	aiReleaseImport(scene);

	this->meshData.invertFaces();

	return true;
}
