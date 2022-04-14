#include "Resources.h"
#include "Graphics/Renderer.h"
#include "Dev/Log.h"

Resources::Resources()
	: renderer(nullptr)
{

}

Resources::~Resources()
{
	// Textures
	for (std::map<std::string, Texture*>::iterator itr = this->textures.begin(), 
		itr_end = this->textures.end(); itr != itr_end; ++itr)
	{
		delete itr->second;
	}

	// Meshes
	for (std::map<std::string, Mesh*>::iterator itr = this->meshes.begin(), itr_end = this->meshes.end(); itr != itr_end; ++itr)
	{
		delete itr->second;
	}

	// Materials
	for (std::map<std::string, Material*>::iterator itr = this->materials.begin(), itr_end = this->materials.end(); itr != itr_end; ++itr)
	{
		delete itr->second;
	}

	// Vertex shaders
	for (std::map<std::string, VertexShader*>::iterator itr = this->vertexShaders.begin(), itr_end = this->vertexShaders.end(); itr != itr_end; ++itr)
	{
		delete itr->second;
	}

	// Pixel shaders
	for (std::map<std::string, PixelShader*>::iterator itr = this->pixelShaders.begin(), itr_end = this->pixelShaders.end(); itr != itr_end; ++itr)
	{
		delete itr->second;
	}
}

void Resources::init(Renderer* renderer)
{
	this->renderer = renderer;
}

void Resources::addTexture(
	const std::string& textureFilePath, const std::string& textureName)
{
	// Create and load texture
	Texture* newTexture = new Texture(*this->renderer);
	newTexture->load(textureFilePath);

	// Insert texture
	this->textures.insert(
		std::pair<std::string, Texture*>(
			textureName,
			newTexture
		)
	);
}

void Resources::addMesh(
	MeshData&& newMeshData, 
	const std::string& meshName)
{
	// Create mesh
	Mesh* newMesh = new Mesh(*this->renderer, std::move(newMeshData));

	// Insert mesh
	this->meshes.insert(
		std::pair<std::string, Mesh*>(
			meshName,
			newMesh
		)
	);
}

void Resources::addMaterial(
	const std::string& diffuseTextureName,
	const std::string materialName
)
{
	// Create material
	Material* newMaterial = new Material(diffuseTextureName);

	// Insert material
	this->materials.insert(
		std::pair<std::string, Material*>(
			materialName,
			newMaterial
		)
	);
}

void Resources::addVertexShader(const std::string& vertexShaderName, InputLayoutDesc& inputLayoutDesc)
{
	// Create and load vertex shader
	VertexShader* createdVertexShader = new VertexShader(*this->renderer);
	createdVertexShader->loadVS(vertexShaderName, inputLayoutDesc);

	// Add the new vertex shader
	this->vertexShaders.insert(
		std::pair<std::string, VertexShader*>(
			vertexShaderName,
			createdVertexShader
		)
	);
}

void Resources::addPixelShader(const std::string& pixelShaderName)
{
	// Create and load pixel shader
	PixelShader* createdPixelShader = new PixelShader(*this->renderer);
	createdPixelShader->loadPS(pixelShaderName);

	// Add the new pixel shader
	this->pixelShaders.insert(
		std::pair<std::string, PixelShader*>(
			pixelShaderName,
			createdPixelShader
		)
	);
}

Texture& Resources::getTexture(const std::string& textureName)
{
	Texture* foundTexture = this->textures[textureName];

	if (!foundTexture)
		Log::error("Texture has not been added as a resource: " + textureName);

	return *foundTexture;
}

Mesh& Resources::getMesh(const std::string& meshName)
{
	Mesh* foundMesh = this->meshes[meshName];

	if (!foundMesh)
		Log::error("Mesh has not been added as a resource: " + meshName);

	return *foundMesh;
}

Material& Resources::getMaterial(const std::string& materialName)
{
	Material* foundMaterial = this->materials[materialName];

	if (!foundMaterial)
		Log::error("Material has not been added: " + materialName);

	return *foundMaterial;
}
