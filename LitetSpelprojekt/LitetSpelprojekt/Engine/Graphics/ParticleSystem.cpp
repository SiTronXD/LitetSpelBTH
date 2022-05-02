#include "ParticleSystem.h"
#include "Renderer.h"
#include "../SMath.h"
#include "../Time.h"
#include <time.h>
#include <stdlib.h>

using namespace DirectX::SimpleMath;

ParticleSystem::ParticleSystem(Renderer& renderer)
	:renderer(renderer),
	structBuffer(renderer, "particleComputeShader"),
	particleComputeShader(renderer, "particleComputeShader"),
	particleVS(renderer),
	particlePS(renderer),
	numberOfParticles(64),
	cPosCbuffer(renderer, "cameraPositionConstantBuffer")
{
}

ParticleSystem::~ParticleSystem()
{
	delete this->plane;
}

void ParticleSystem::init()
{
	//Init particles
	this->initParticles();
	
	//Init plane
	MeshData data(DefaultMesh::PLANE);
	data.transformMesh(Matrix::CreateRotationX(SMath::PI * 0.5));
	this->plane = new Mesh(this->renderer, std::move(data));

	//Init structured buffer
	this->structBuffer.createBuffer(sizeof(Particle), numberOfParticles, this->particles.data());

	//Constant buffers
	this->cPosCbuffer.createBuffer(sizeof(CameraStruct));

	//Set inputlayout
	InputLayoutDesc inputLayoutDesc;
	inputLayoutDesc.add("POSITION", DXGI_FORMAT_R32G32B32A32_FLOAT);
	inputLayoutDesc.add("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT);
	inputLayoutDesc.add("UV", DXGI_FORMAT_R32G32_FLOAT);

	inputLayoutDesc.add("SV_InstanceID", DXGI_FORMAT_R32_UINT);
	
	//Init shaders
	this->particleVS.loadVS("Particle_VS", inputLayoutDesc);
	this->particlePS.loadPS("Particle_PS");

	//Init compute shader
	this->particleComputeShader.init("Particle_COMP", (this->numberOfParticles / 32),1,1);
	this->particleComputeShader.addUAV(this->structBuffer.getUav());
	this->particleComputeShader.addConstantBuffer(this->cPosCbuffer);
}

void ParticleSystem::startParticleSystem(float x, float y, float z)
{
	//Update values on particles
	
	
	this->active = true;
}

void ParticleSystem::update(const DirectX::XMFLOAT3& cameraPosition)
{
}

void ParticleSystem::render(DirectX::SimpleMath::Matrix& vp, const DirectX::XMFLOAT3& cameraPosition)
{
	if (Input::isKeyJustPressed(Keys::E))
	{
		this->startParticleSystem(0.0f, -6.0f, 0.0f);
	}
	
	if (this->active)
	{
		this->particleComputeShader.run();

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

		//Update constant buffer
		this->cameraStruct.position = cameraPosition;
		this->cameraStruct.deltaTime = Time::getDT();
		this->cPosCbuffer.updateBuffer(&this->cameraStruct);

		//Bind constant buffer
		this->renderer.getCameraBufferStruct().modelMat = this->m.Transpose();
		this->renderer.getCameraConstantBuffer().updateBuffer(&this->renderer.getCameraBufferStruct());

		deviceContext->VSSetConstantBuffers(0, 1, &this->renderer.getCameraConstantBuffer().getBuffer());

		//Bind structur buffer
		deviceContext->VSSetShaderResources(0, 1, &this->structBuffer.getSrv().getPtr());

		//Draw
		deviceContext->DrawIndexedInstanced(this->plane->getIndexBuffer().getIndexCount(),
			this->numberOfParticles,
			0, 0, 0
		);

		ID3D11ShaderResourceView* nullsrv[] = { nullptr };
		deviceContext->VSSetShaderResources(0, 1, nullsrv);
	}
}

void ParticleSystem::initParticles()
{
	//Create an vector of particles and init them
	for (int i = 0; i < this->numberOfParticles; i++)
	{
		Particle particle = {};

		this->m = Matrix::CreateScale(1.0f,1.0f,1.0f) *
			Matrix::CreateRotationX(0.0f) *
			Matrix::CreateRotationY(1.0f) *
			Matrix::CreateRotationZ(0.0f) *
			Matrix::CreateTranslation(0.0f, -6.0f, 0.0f);
		
		particle.worldMatrix = this->m.Transpose();

		particle.scaleFactor = 1;
		
		particle.velocity.x = 0;
		particle.velocity.y = 0;
		particle.velocity.z = 0;
		particle.lifetime = 0;

		this->renderer.getCameraBufferStruct().modelMat = this->m.Transpose();
		this->particles.push_back(particle);
	}
}

void ParticleSystem::killParticles()
{
	
}
