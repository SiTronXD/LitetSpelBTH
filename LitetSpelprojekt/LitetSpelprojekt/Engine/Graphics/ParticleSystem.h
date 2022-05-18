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

enum class System
{
	STOP		= 0,
	EXPLOSION	= 1,
	LOOP		= 2
};

class ParticleSystem
{
private:
	struct Particle
	{
		DirectX::XMFLOAT4X4 worldMatrix;
		DirectX::XMFLOAT3 velocity;
		float lifetime;
		float scaleFactor;
		DirectX::XMFLOAT3 finalColor;
	}particleBufferStruct{};

	struct ParticleSystemStruct
	{
		DirectX::XMFLOAT3 cameraPosition;
		float deltaTime;
		DirectX::XMFLOAT3 startPosition;
		int32_t start;
		DirectX::XMFLOAT3 color1;
		float speed;
		DirectX::XMFLOAT3 color2;
		float lifeTime;
		float randomTimer;
		int particleType;
		DirectX::XMFLOAT2 padding;

	}particleSystemStruct{};

	Mesh* plane;
	Renderer* renderer;
	Resources* resource;
	Texture* texture;

	std::vector<Particle> particles;

	StructuredBuffer* structBuffer;
	ComputeShader* particleComputeShader;

	DirectX::SimpleMath::Matrix m;

	ConstantBuffer* cPosCbuffer;

	VertexShader* particleVS;
	PixelShader* particlePS;

	void setParticleType(System system);
	void initParticles(DirectX::SimpleMath::Vector3 position, float speed, float lifetime);

	int numberOfParticles;

	float activeTimer;

public:
	ParticleSystem();
	~ParticleSystem();

	void init(Renderer& renderer, Resources& resource, int nrOfParticles);
	void setColor(DirectX::SimpleMath::Vector3 color1, DirectX::SimpleMath::Vector3 color2);

	void explode(DirectX::SimpleMath::Vector3 position, float speed, float lifetime);
	void loopable(DirectX::SimpleMath::Vector3 position, float speed, float lifetime);
	void stop();

	void render(DirectX::SimpleMath::Matrix& vp, const DirectX::XMFLOAT3& cameraPosition);

private:
	void initParticles();
};

