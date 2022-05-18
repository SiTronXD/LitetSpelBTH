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

	// Cubemaps
	for (std::map<std::string, CubeMap*>::iterator itr = this->cubeMaps.begin(), itr_end = this->cubeMaps.end(); itr != itr_end; ++itr)
	{
		delete itr->second;
	}

	// SoundEffects
	for (std::map<std::string, sf::SoundBuffer*>::iterator itr = this->soundEffects.begin(), itr_end = this->soundEffects.end(); itr != itr_end; ++itr)
	{
		delete itr->second;
	}
}

void Resources::init(Renderer* renderer)
{
	this->renderer = renderer;
}

void Resources::update()
{
	// Update animations in meshes
	for (std::map<std::string, Mesh*>::iterator itr = this->meshes.begin(), itr_end = this->meshes.end(); 
		itr != itr_end; 
		++itr)
	{
		itr->second->update();
	}
}

void Resources::addTexture(
	const std::string& textureFilePath, const std::string& textureName,
	bool saveImageData)
{
	if (this->textures.count(textureName) > 0)
	{
		Log::write(textureName + " has already been added to resources.");
		return;
	}

	// Create and load texture
	Texture* newTexture = new Texture(*this->renderer);
	newTexture->load(textureFilePath, saveImageData);

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
	if (this->meshes.count(meshName) > 0)
	{
		Log::write(meshName + " has already been added to resources.");
		return;
	}

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
	if (this->materials.count(materialName) > 0)
	{
		Log::write(materialName + " has already been added to resources.");
		return;
	}

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
	if (this->vertexShaders.count(vertexShaderName) > 0)
	{
		Log::write(vertexShaderName + " has already been added to resources.");
		return;
	}

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
	if (this->pixelShaders.count(pixelShaderName) > 0)
	{
		Log::write(pixelShaderName + " has already been added to resources.");
		return;
	}

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

void Resources::addCubeMap(const std::string& textureFileName, const std::string& textureFormat, const std::string& cubeMapName)
{
	//Check if resource already exist
	if (this->cubeMaps.count(cubeMapName) > 0)
	{
		Log::write(cubeMapName + " has already been added to resources.");
		return;
	}

	//Create and load cubemap
	CubeMap* createdCubeMap = new CubeMap(*this->renderer);
	createdCubeMap->createCubeMap(textureFileName, textureFormat);

	//Insert cubemap
	this->cubeMaps.insert(
		std::pair<std::string, CubeMap*>(
			cubeMapName,
			createdCubeMap
			)
	);
}

void Resources::addSoundEffect(const std::string& soundFileName, const std::string& soundName)
{
	//Check if resource already exist
	if (this->soundEffects.count(soundFileName) > 0)
	{
		Log::write(soundFileName + " has already been added to resources.");
		return;
	}

	//Create sound effect from file
	sf::SoundBuffer* buffer = new sf::SoundBuffer();
	if (!buffer->loadFromFile(soundFileName))
	{
		Log::write("Failed to load sound file: " + soundFileName);
	}

	//Insert sound effect
	this->soundEffects.insert(
		std::pair<std::string, sf::SoundBuffer*>(
			soundName,
			buffer
			)
	);
}

Texture& Resources::getTexture(const char* textureName)
{
	Texture* foundTexture = this->textures[textureName];

	if (!foundTexture)
		Log::error("Texture has not been added as a resource: " + std::string(textureName));

	return *foundTexture;
}

Mesh& Resources::getMesh(const char* meshName)
{
	Mesh* foundMesh = this->meshes[meshName];

	if (!foundMesh)
		Log::error("Mesh has not been added as a resource: " + std::string(meshName));

	return *foundMesh;
}

Material& Resources::getMaterial(const char* materialName)
{
	Material* foundMaterial = this->materials[materialName];

	if (!foundMaterial)
		Log::error("Material has not been added: " + std::string(materialName));

	return *foundMaterial;
}

VertexShader& Resources::getVertexShader(const char* vertexShaderName)
{
	VertexShader* foundVertexShader = this->vertexShaders[vertexShaderName];

	if (!foundVertexShader)
		Log::error("Vertex shader has not been added: " + std::string(vertexShaderName));

	return *foundVertexShader;
}

PixelShader& Resources::getPixelShader(const char* pixelShaderName)
{
	PixelShader* foundPixelShader = this->pixelShaders[pixelShaderName];

	if (!foundPixelShader)
		Log::error("Pixel shader has not been added: " + std::string(pixelShaderName));

	return *foundPixelShader;
}

CubeMap& Resources::getCubemap(const char* cubemapName)
{
	CubeMap* foundMaterial = this->cubeMaps[cubemapName];

	if (!foundMaterial)
		Log::error("Cubemap has not been added: " + std::string(cubemapName));

	return *foundMaterial;
}

sf::SoundBuffer& Resources::getSoundEffect(const char* soundEffectName)
{
	sf::SoundBuffer* soundEffect = this->soundEffects[soundEffectName];

	if (!soundEffect)
		Log::error("Sound Effect has not been added: " + std::string(soundEffectName));

	return *soundEffect;
}
