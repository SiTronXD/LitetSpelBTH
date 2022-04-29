#include "ParticleSystem.h"
#include "Renderer.h"

ParticleSystem::ParticleSystem(Renderer& renderer)
	:renderer(renderer),
	structBuffer(renderer, "particleComputeShader"),
	particleComputeShader(renderer, "particleComputeShader"),
	particleVS(renderer),
	particlePS(renderer),
	numberOfParticles(1),
	testBuffer(renderer, "particleConstantBuffer")
{
}

ParticleSystem::~ParticleSystem()
{
	delete this->plane;
	//this->testBuffer.getBuffer()->Release();
}

void ParticleSystem::init()
{
	//Init plane
	MeshData data(DefaultMesh::PLANE);
	this->plane = new Mesh(this->renderer, std::move(data));

	//Create an vector of particles
	for (int i = 0; i < this->numberOfParticles; i++)
	{
		Particle particle = {};
		this->m = DirectX::XMMatrixScaling(1.0f, 1.0f, 1.0f) *
			DirectX::XMMatrixRotationRollPitchYaw(10.0f, -8.0f, -6.0f) *
			DirectX::XMMatrixTranslation(10.0f, -6.0f, 10.0f);

		particle.worldMatrix = this->m;
		this->particles.push_back(particle);
	}

	//Init structured buffer
	this->structBuffer.createBuffer(sizeof(Particle), numberOfParticles, this->particles.data());

	//Set inputlayout
	InputLayoutDesc inputLayoutDesc;
	inputLayoutDesc.add("POSITION", DXGI_FORMAT_R32G32B32A32_FLOAT);
	
	//Init shaders
	this->particleVS.loadVS("Particle_VS", inputLayoutDesc);
	this->particlePS.loadPS("Particle_PS");

	//Init compute shader
	//this->particleComputeShader.init("Particle_COMP", (this->numberOfParticles / 1))
}

void ParticleSystem::update()
{
}

void ParticleSystem::render(DirectX::SimpleMath::Matrix& vp, const DirectX::SimpleMath::Vector3& cameraRot)
{
	//Update world,view and proj matrix.
	this->m = this->m * vp;

	this->particles[0].worldMatrix = this->m.Transpose();

	//Bind pipeline
	ID3D11DeviceContext* deviceContext = this->renderer.getDeviceContext();
	deviceContext->IASetInputLayout(this->particleVS.getInputLayout());

	deviceContext->IASetVertexBuffers(
		0, 1,
		&this->plane->getVertexBuffer().getBuffer(),
		&this->plane->getVertexBuffer().getStride(),
		&this->plane->getVertexBuffer().getOffset()

	);

	deviceContext->IASetIndexBuffer(this->plane->getIndexBuffer().getBuffer(),
		DXGI_FORMAT_R32_UINT, 0
	);

	//Bind vertex/pixel shader
	deviceContext->VSSetShader(this->particleVS.getVS(), nullptr, 0);
	deviceContext->PSSetShader(this->particlePS.getPS(), nullptr, 0);

	//Bind structur buffer
	//deviceContext->VSSetShaderResources(0, 1, &this->structBuffer.getBuffer());

	deviceContext->DrawIndexed(this->plane->getIndexBuffer().getIndexCount(), 0, 0);
	//deviceContext->DrawIndexedInstanced()
}
