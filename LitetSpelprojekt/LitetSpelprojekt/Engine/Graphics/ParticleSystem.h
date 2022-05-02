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

class ParticleSystem
{
private:
	struct Particle
	{
		DirectX::XMFLOAT4X4 worldMatrix;
		DirectX::XMFLOAT3 velocity;
		float lifetime;
		float scaleFactor;
		DirectX::XMFLOAT3 padding;
	}particleBufferStruct{};

	struct CameraStruct
	{
		DirectX::XMFLOAT3 cameraPosition;
		float deltaTime;
		DirectX::XMFLOAT3 startPosition;
		int start;
	}cameraStruct{};

	Mesh* plane;
	float timeTest;

	DirectX::SimpleMath::Matrix m;

	std::vector<Particle> particles;

	StructuredBuffer structBuffer;
	ComputeShader particleComputeShader;

	ConstantBuffer cPosCbuffer;

	VertexShader particleVS;
	PixelShader particlePS;

	Renderer& renderer;

	int numberOfParticles;

	bool active;
public:
	ParticleSystem(Renderer&  renderer);
	~ParticleSystem();

	void init();

	void startParticleSystem(float x, float y, float z);

	void update(const DirectX::XMFLOAT3& cameraPosition);
	void render(DirectX::SimpleMath::Matrix& vp, const DirectX::XMFLOAT3& cameraPosition);

private:
	void initParticles();
	void killParticles();
};

