#include "LevelLoader.h"
#include "../../Engine/Dev/Log.h"

#include <DirectXMath.h>

using namespace DirectX;
using namespace DirectX::SimpleMath;

void LevelLoader::traverseFile(aiNode* node)
{
	Log::write("Name: " + std::string(node->mName.C_Str()));

	for(int i = 0; i < node->mNumChildren; ++i)
		this->traverseFile(node->mChildren[i]);
}

LevelLoader::LevelLoader()
	: playerStartPos(0,0,0)
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

	// aiNode* rootNode = scene->mRootNode;
	// this->traverseFile(rootNode);

	Log::write("Num materials: " + std::to_string(scene->mNumMaterials));
	for (int i = 0; i < scene->mNumMaterials; ++i)
	{
		Log::write("Mat: ");

		aiString path;
		if (scene->mMaterials[i]->GetTexture(
			aiTextureType_DIFFUSE,
			0,
			&path,
			NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
		{
			Log::write("path: " + std::string(path.C_Str()));
		}
	}

	// Loop through each submesh
	unsigned int indexOffset = 0;
	for (unsigned int i = 0; i < scene->mNumMeshes; ++i)
	{
		aiMesh* submesh = scene->mMeshes[i];

		std::string submeshName = std::string(submesh->mName.C_Str());

		// Colliders
		if (submeshName.find("collider_") != std::string::npos)
		{

		}
		// Player start position
		else if (submeshName == "PlayerStartPos")
		{
			// Average position
			Vector3 sumPos = Vector3(0, 0, 0);
			for (unsigned int j = 0; j < submesh->mNumVertices; ++j)
			{
				sumPos += Vector3(
					submesh->mVertices[j].x,
					submesh->mVertices[j].y,
					submesh->mVertices[j].z
				);
			}
			sumPos /= (float) submesh->mNumVertices;

			// Apply
			this->playerStartPos = sumPos;
		}
		// Mesh
		else
		{
			Log::write("Material index: " + std::to_string(submesh->mMaterialIndex));

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
			for (unsigned int j = 0; j < submesh->mNumFaces; ++j)
			{
				if (submesh->mFaces[j].mNumIndices != 3u)
					continue;

				this->meshData.addIndex(indexOffset + submesh->mFaces[j].mIndices[0]);
				this->meshData.addIndex(indexOffset + submesh->mFaces[j].mIndices[1]);
				this->meshData.addIndex(indexOffset + submesh->mFaces[j].mIndices[2]);
			
				numIndices += 3;
			}

			Submesh newSubmesh{};
			newSubmesh.startIndex = indexOffset;
			newSubmesh.numIndices = numIndices;
			this->meshData.addSubmesh(newSubmesh);

			// Update offset for next submesh
			indexOffset = this->meshData.getVertices().size();
		}
	}

	aiReleaseImport(scene);

	this->meshData.invertFaces();

	return true;
}
