#pragma once

#include <string>
#include <map>

#include "Graphics/Texture.h"
#include "Graphics/Mesh.h"
#include "Graphics/Material.h"
#include "Graphics/Shaders/VertexShader.h"
#include "Graphics/Shaders/PixelShader.h"

class Renderer;

class Resources
{
private:
	std::map<std::string, Texture*> textures;
	std::map<std::string, Mesh*> meshes;
	std::map<std::string, Material*> materials;
	std::map<std::string, VertexShader*> vertexShaders;
	std::map<std::string, PixelShader*> pixelShaders;

	Renderer* renderer;

public:
	Resources();
	virtual ~Resources();

	void init(Renderer* renderer);

	void addTexture(
		const std::string& textureFilePath,
		const std::string& textureName
	);
	void addMesh(
		MeshData&& newMeshData, 
		const std::string& meshName
	);
	void addMaterial(
		const std::string& diffuseTextureName,
		const std::string materialName
	);
	void addVertexShader(
		const std::string& vertexShaderName,
		InputLayoutDesc& inputLayoutDesc
	);
	void addPixelShader(
		const std::string& pixelShaderName
	);

	Texture& getTexture(const char* textureName);
	Mesh& getMesh(const char* meshName);
	Material& getMaterial(const char* materialName);
};