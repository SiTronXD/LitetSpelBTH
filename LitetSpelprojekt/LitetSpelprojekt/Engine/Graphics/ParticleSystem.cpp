#include "ParticleSystem.h"
#include "Renderer.h"
#include "../SMath.h"
#include "../Time.h"
#include <time.h>
#include <stdlib.h>

using namespace DirectX::SimpleMath;

ParticleSystem::ParticleSystem()
	:numberOfParticles(64)
{
}

ParticleSystem::~ParticleSystem()
{
	delete this->plane;
	delete this->particleVS;
	delete this->particlePS;
	delete this->structBuffer;
	delete this->cPosCbuffer;
	delete this->particleComputeShader;

}

void ParticleSystem::init(Renderer& renderer, Resources& resource, int nrOfParticles)
{
	this->renderer = &renderer;
	this->resource = &resource;
	this->numberOfParticles = nrOfParticles;
	
	if (this->particleVS != nullptr)
	{
		delete this->particleVS;
		this->particleVS = nullptr;
	}

	if (this->particlePS != nullptr)
	{
		delete this->particlePS;
		this->particlePS = nullptr;
	}

	if (this->structBuffer != nullptr)
	{
		delete this->structBuffer;
		this->structBuffer = nullptr;
	}

	if (this->cPosCbuffer != nullptr)
	{
		delete this->cPosCbuffer;
		this->cPosCbuffer = nullptr;
	}

	if (this->particleComputeShader != nullptr)
	{
		delete this->particleComputeShader;
		this->particleComputeShader = nullptr;
	}

	this->particleVS = new VertexShader(renderer);
	this->particlePS = new PixelShader(renderer);
	this->structBuffer = new StructuredBuffer(renderer, "particleStructuredbuffer");
	this->cPosCbuffer = new ConstantBuffer(renderer, "particleConstantbuffer");
	this->particleComputeShader = new ComputeShader(renderer, "particleComputeShader");

	//Init particles
	this->initParticles();

	//load texture
	this->texture = &this->resource->getTexture("particle.png");
	
	//Init plane
	MeshData data(DefaultMesh::PLANE);
	data.transformMesh(Matrix::CreateScale(0.25f, 0.25f, 0.25f));
	data.transformMesh(Matrix::CreateRotationX(SMath::PI * 0.5));
	this->plane = new Mesh(renderer, std::move(data));


	//Init structured buffer
	this->structBuffer->createBuffer(sizeof(Particle), numberOfParticles, this->particles.data());

	//Constant buffers
	this->cPosCbuffer->createBuffer(sizeof(CameraStruct));

	//Set inputlayout
	InputLayoutDesc inputLayoutDesc;
	inputLayoutDesc.add("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	inputLayoutDesc.add("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT);
	inputLayoutDesc.add("UV", DXGI_FORMAT_R32G32_FLOAT);

	inputLayoutDesc.add("SV_InstanceID", DXGI_FORMAT_R32_UINT);
	
	//Init shaders
	this->particleVS->loadVS("Particle_VS", inputLayoutDesc);
	this->particlePS->loadPS("Particle_PS");

	//Init compute shader
	this->particleComputeShader->init("Particle_COMP", (this->numberOfParticles / 32),1,1);
	this->particleComputeShader->addUAV(this->structBuffer->getUav());
	this->particleComputeShader->addConstantBuffer(*this->cPosCbuffer);
}

void ParticleSystem::explode(DirectX::SimpleMath::Vector3 position, float speed, float lifetime)
{
	this->cameraStruct.startPosition = position;
	this->cameraStruct.speed = speed;
	this->cameraStruct.lifeTime = lifetime;
	this->cameraStruct.start = 1;

	this->active = true;
}

void ParticleSystem::render(DirectX::SimpleMath::Matrix& vp, const DirectX::XMFLOAT3& cameraPosition)
{
	if (this->active)
	{
		this->particleComputeShader->run();

		//Bind pipeline
		ID3D11DeviceContext* deviceContext = this->renderer->getDeviceContext();
		deviceContext->IASetInputLayout(this->particleVS->getInputLayout());

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
		deviceContext->VSSetShader(this->particleVS->getVS(), nullptr, 0);
		deviceContext->PSSetShader(this->particlePS->getPS(), nullptr, 0);

		//Update constant buffer
		this->cameraStruct.cameraPosition = cameraPosition;
		this->cameraStruct.deltaTime = Time::getDT();
		this->cPosCbuffer->updateBuffer(&this->cameraStruct);

		//Bind constant buffer
		this->renderer->getCameraBufferStruct().modelMat = this->m.Transpose();
		this->renderer->getCameraConstantBuffer().updateBuffer(&this->renderer->getCameraBufferStruct());

		deviceContext->VSSetConstantBuffers(0, 1, &this->renderer->getCameraConstantBuffer().getBuffer());

		//Bind structur buffer
		deviceContext->VSSetShaderResources(0, 1, &this->structBuffer->getSrv().getPtr());

		//Samplestate
		deviceContext->PSSetSamplers(
			0, 1, &this->texture->getSampler()
		);
		deviceContext->PSSetShaderResources(
			0, 1, &this->texture->getSRV().getPtr()
		);

		//Draw
		deviceContext->DrawIndexedInstanced(this->plane->getIndexBuffer().getIndexCount(),
			this->numberOfParticles,
			0, 0, 0
		);

		ID3D11ShaderResourceView* nullsrv[] = { nullptr };
		deviceContext->VSSetShaderResources(0, 1, nullsrv);

		this->cameraStruct.start = 0;
	}
}

void ParticleSystem::initParticles()
{
	this->m = Matrix::CreateScale(1.0f, 1.0f, 1.0f) *
		Matrix::CreateRotationX(0.0f) *
		Matrix::CreateRotationY(1.0f) *
		Matrix::CreateRotationZ(0.0f) *
		Matrix::CreateTranslation(0.0f, 0.0f, 0.0f);
	
	//Create an vector of particles and init them
	for (int i = 0; i < this->numberOfParticles; i++)
	{
		Particle particle = {};

		particle.worldMatrix = this->m.Transpose();

		this->renderer->getCameraBufferStruct().modelMat = this->m.Transpose();
		this->particles.push_back(particle);
	}
}

void ParticleSystem::stopParticleSystem()
{
	
}
