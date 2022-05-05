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
		DirectX::XMMATRIX worldMatrix;
	}particleBufferStruct{};

	Mesh* plane;

	DirectX::SimpleMath::Matrix m;

	std::vector<Particle> particles;

	StructuredBuffer structBuffer;
	ComputeShader particleComputeShader;

	ConstantBuffer testBuffer;

	VertexShader particleVS;
	PixelShader particlePS;

	Renderer& renderer;

	int numberOfParticles;

public:
	ParticleSystem(Renderer&  renderer);
	~ParticleSystem();

	void init();

	void update();
	void render(DirectX::SimpleMath::Matrix& vp, const DirectX::SimpleMath::Vector3& cameraRot);

};

