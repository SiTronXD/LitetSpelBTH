#include "Skybox.h"
#include "../Graphics/Texture.h"
#include "../Dev/Log.h"
#include "InputLayoutDesc.h"
#include "../Application/Scene.h"

Skybox::Skybox(Renderer& renderer)
	:cubemap(renderer), vertexShader(renderer), pixelShader(renderer), constantBuffer(renderer, "constantBuffer"), renderer(renderer)
{
}

Skybox::~Skybox()
{
	delete this->cube;
}

void Skybox::update(const DirectX::SimpleMath::Vector3& cameraPos, const DirectX::XMMATRIX& vpMatrix)
{
	//Update constant buffer
	
	//First reset skybox
	DirectX::SimpleMath::Matrix m = DirectX::XMMatrixIdentity();
	//DirectX::SimpleMath::Matrix scale = DirectX::XMMatrixScaling(10.0f, 10.0f, 10.0f);

	m = vpMatrix;

	//Define the skybox in worldspace
	this->bufferStruct.mvpMat = m.Transpose();
	
	this->bufferStruct.cameraPos = cameraPos;
	
	this->constantBuffer.updateBuffer(&this->bufferStruct);
	
}

void Skybox::initialize()
{

	//Initialize a skybox texture.
	this->cubemap.createCubeMap("Skybox");
	MeshData data(DefaultMesh::CUBE);
	data.invertFaces();

	this->cube = new Mesh(this->renderer, std::move(data));

	//Shaders
	this->initShaders();

	this->constantBuffer.createBuffer(sizeof(BufferData));
}

void Skybox::shutdown()
{
}

void Skybox::initShaders()
{
	// Input layout desc
	InputLayoutDesc inputLayoutDesc;
	inputLayoutDesc.add("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	inputLayoutDesc.add("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT);
	inputLayoutDesc.add("UV", DXGI_FORMAT_R32G32_FLOAT);

	// Vertex shader
	this->vertexShader.loadVS("skybox_VS", inputLayoutDesc);
	
	// Pixel shader
	this->pixelShader.loadPS("skybox_PS");

}
