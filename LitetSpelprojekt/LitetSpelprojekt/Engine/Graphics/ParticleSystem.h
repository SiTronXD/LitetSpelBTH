#pragma once
#include "Mesh.h"
#include "MeshData.h"
#include "StructuredBuffer.h"
#include "Shaders/ComputeShader.h"
#include "Shaders/VertexShader.h"
#include "Shaders/PixelShader.h"
#include <SimpleMath.h>
#include <vector>
#include "ConstantBuffer.h"
#include "Texture.h"
#include "../Resources.h"

class ParticleSystem
{
private:
	struct Particle
	{
		DirectX::XMFLOAT4X4 worldMatrix;
		DirectX::XMFLOAT3 velocity;
		float lifetime;
		float scaleFactor;
		DirectX::XMFLOAT3 color;
	}particleBufferStruct{};

	struct ParticleSystemStruct
	{
		DirectX::XMFLOAT3 cameraPosition;
		float deltaTime;
		DirectX::XMFLOAT3 startPosition;
		int32_t start;
		float speed;
		float lifeTime;
		DirectX::XMFLOAT2 padding;

	}particleSystemStruct{};

	Mesh* plane;
	Renderer* renderer;
	Resources* resource;
	Texture* texture;

	DirectX::SimpleMath::Matrix m;

	std::vector<Particle> particles;

	StructuredBuffer* structBuffer;
	ComputeShader* particleComputeShader;

	ConstantBuffer* cPosCbuffer;

	VertexShader* particleVS;
	PixelShader* particlePS;

	int numberOfParticles;

	bool active;
public:
	ParticleSystem();
	~ParticleSystem();

	void init(Renderer& renderer, Resources& resource, int nrOfParticles);

	void explode(DirectX::SimpleMath::Vector3 position, float speed, float lifetime);

	void render(DirectX::SimpleMath::Matrix& vp, const DirectX::XMFLOAT3& cameraPosition);

private:
	void initParticles();
};

