#pragma once

#include <string>
#include <map>
#include "SFML/Audio.hpp"

#include "Graphics/Texture.h"
#include "Graphics/Mesh.h"
#include "Graphics/Material.h"
#include "Graphics/Shaders/VertexShader.h"
#include "Graphics/Shaders/PixelShader.h"
#include "Graphics/CubeMap.h"

class Renderer;

class Resources
{
private:
	std::map<std::string, Texture*> textures;
	std::map<std::string, Mesh*> meshes;
	std::map<std::string, Material*> materials;
	std::map<std::string, VertexShader*> vertexShaders;
	std::map<std::string, PixelShader*> pixelShaders;
	std::map<std::string, CubeMap*> cubeMaps;
	std::map<std::string, sf::SoundBuffer*> soundEffects;

	Renderer* renderer;

public:
	Resources();
	virtual ~Resources();

	void init(Renderer* renderer);

	void addTexture(
		const std::string& textureFilePath,
		const std::string& textureName,
		bool saveImageData = false
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

	void addCubeMap(
		const std::string& textureFileName,
		const std::string& textureFormat,
		const std::string& cubeMapName
	);

	void addSoundEffect(
		const std::string& soundFileName,
		const std::string& soundName
	);

	Texture& getTexture(const char* textureName);
	Mesh& getMesh(const char* meshName);
	Material& getMaterial(const char* materialName);
	VertexShader& getVertexShader(const char* vertexShaderName);
	CubeMap& getCubemap(const char* cubemapName);
	sf::SoundBuffer& getSoundEffect(const char* soundEffectName);
};