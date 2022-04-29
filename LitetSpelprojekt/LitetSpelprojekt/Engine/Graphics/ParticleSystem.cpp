#include "ParticleSystem.h"
#include "Renderer.h"

using namespace DirectX::SimpleMath;

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
		this->m = Matrix::CreateScale(1.0f, 1.0f, 1.0f) *
			Matrix::CreateRotationX(10.0f) *
			Matrix::CreateRotationY(-8.0f) *
			Matrix::CreateRotationZ(1.0f) *
			Matrix::CreateTranslation(7.0f, -8.0f, 6.0f);
		particle.worldMatrix = this->m.Transpose();

		this->renderer.getCameraBufferStruct().modelMat = this->m.Transpose();
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

#include <ctime>

void ParticleSystem::render(DirectX::SimpleMath::Matrix& vp, const DirectX::SimpleMath::Vector3& cameraRot)
{
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

	//Bind constant buffer
	
	this->renderer.getCameraBufferStruct().modelMat = this->m.Transpose();
	this->renderer.getCameraConstantBuffer().updateBuffer(&this->renderer.getCameraBufferStruct());
	deviceContext->VSSetConstantBuffers(0, 1, &this->renderer.getCameraConstantBuffer().getBuffer());

	/*this->timeTest += 0.001;

	this->m = Matrix::CreateScale(1.0f, 1.0f, 1.0f) *
		Matrix::CreateRotationX(10.0f) *
		Matrix::CreateRotationY(-8.0f) *
		Matrix::CreateRotationZ(1.0f) *
		Matrix::CreateTranslation(7.0f, -8.0f, 6.0f);
	
	this->particles[0].worldMatrix = m.Transpose();*/

	this->structBuffer.updateBuffer(&this->particleBufferStruct);

	//Bind structur buffer
	deviceContext->VSSetShaderResources(0, 1, &this->structBuffer.getSrv().getPtr());

	deviceContext->DrawIndexed(this->plane->getIndexBuffer().getIndexCount(), 0, 0);
	//deviceContext->DrawIndexedInstanced()
}
