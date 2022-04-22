#include "Mesh.h"
#include "MeshData.h"
#include "Shaders/VertexShader.h"
#include "Shaders/PixelShader.h"
#include "CubeMap.h"
#include "ConstantBuffer.h"

class Skybox
{
private:
	struct BufferData
	{
		DirectX::XMFLOAT4X4 mvpMat;
		DirectX::SimpleMath::Vector3 cameraPos;
		float fill;
	} bufferStruct{};
	
	Renderer& renderer;
	
	//Texture
	CubeMap cubemap;
	
	//Mesh cube
	Mesh* cube;

	//Shaders
	VertexShader vertexShader;
	PixelShader pixelShader;

	//Buffers
	ConstantBuffer constantBuffer;

public:
	Skybox(Renderer& renderer);
	~Skybox();

	inline Mesh& getMesh() { return *this->cube; }
	inline CubeMap& getCubeMap() { return this->cubemap; }
	
	inline ConstantBuffer& getConstantBuffer() { return this->constantBuffer; }
	inline VertexShader& getVertexShader() { return this->vertexShader; }
	inline PixelShader& getPixelShader() { return this->pixelShader; }

	void update(const DirectX::SimpleMath::Vector3& cameraPos, const DirectX::XMMATRIX& vpMatrix);

	void initialize();
	void shutdown();

private:
	void initShaders();

};