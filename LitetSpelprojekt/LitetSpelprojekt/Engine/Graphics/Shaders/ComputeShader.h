#pragma once

#include <string>
#include <vector>
#include <d3d11.h>
#include "Shader.h"
#include "../Texture.h"
#include "../ConstantBuffer.h"
#include "../UAV.h"
#include "../SRV.h"

class Renderer;

class ComputeShader : public Shader
{
private:
	static const int NUM_MAX_UAV = 8;
	static const int NUM_MAX_SRV = 8;
	static const int NUM_MAX_RENDER_TEXTURES = 8;
	static const int NUM_MAX_CONSTANT_BUFFERS = 4;

	ID3D11ComputeShader* computeShader;

	// Arrays filled with NULL for resetting current views in compute shader after dispatch
	ID3D11UnorderedAccessView* uavNULL[NUM_MAX_UAV];
	ID3D11ShaderResourceView* srvNULL[NUM_MAX_SRV];
	ID3D11Buffer* constantBufferNULL[NUM_MAX_CONSTANT_BUFFERS];

	std::vector<Texture*> renderTextures;
	std::vector<ID3D11UnorderedAccessView*> unorderedAccessViews;
	std::vector<ID3D11ShaderResourceView*> shaderResourceViews;
	std::vector<ID3D11Buffer*> constantBuffers;

	std::string debugName;

	unsigned int threadGroupX;
	unsigned int threadGroupY;
	unsigned int threadGroupZ;

	bool createFromFile(const std::string& fileName);

public:
	ComputeShader(Renderer& renderer, const std::string& debugName);
	~ComputeShader();

	void init(const std::string& fileName,
		unsigned int threadGroupX, unsigned int threadGroupY, unsigned int threadGroupZ);

	void run();

	void setThreadGroup(unsigned int threadGroupX, unsigned int threadGroupY, unsigned int threadGroupZ);

	// Preset number of resources
	void setNumConstantBuffers(unsigned int num);
	void setNumUAVs(unsigned int num);
	void setNumSRVs(unsigned int num);
	void setNumRenderTextures(unsigned int num);

	// Add resource into vector
	void addConstantBuffer(ConstantBuffer& buffer, bool allowNullptr = false);
	void addUAV(UAV& uavToAdd, bool allowNullptr = false);
	void addSRV(SRV& srvToAdd, bool allowNullptr = false);
	// void addRenderTexture(Texture& texture);

	// Set resource at a specific index
	void setConstantBuffer(unsigned int index, ConstantBuffer& bufferToSet);
	void setUAV(unsigned int index, UAV& uavToSet);
	void setSRV(unsigned int index, SRV& srvToSet);
	// void setRenderTextures(unsigned int index, Texture& textureToSet);
};