#include "ComputeShader.h"
#include "../../Dev/Helpers.h"
#include "../../Dev/Log.h"
#include "../Renderer.h"

bool ComputeShader::createFromFile(const std::string& fileName)
{
	// Deallocate old texture, if it exists
	S_RELEASE(this->computeShader);

	// Only accept shader model 5.0 for best performance and features
	if (this->getRenderer().getDevice()->GetFeatureLevel() < D3D_FEATURE_LEVEL_11_0)
	{
		Log::error("Shader model 5.0 is not supported on this device. Compute shader will not be loaded.");

		return false;
	}

	// Load and fill with bytecode
	std::vector<char> csData;
	this->loadFile(fileName, csData);

	// Create compute shader
	HRESULT result = this->getRenderer().getDevice()->CreateComputeShader(
		csData.data(), csData.size(), nullptr, &this->computeShader
	);
	if (FAILED(result))
	{
		Log::resultFailed("Failed creating compute shader: " + this->debugName, result);

		return false;
	}

	return true;
}

ComputeShader::ComputeShader(Renderer& renderer, const std::string& debugName)
	: Shader(renderer), computeShader(nullptr),
	uavNULL{ NULL }, srvNULL{ NULL }, constantBufferNULL{ NULL },
	debugName(debugName),
	threadGroupX(0), threadGroupY(0), threadGroupZ(0)
{
}

ComputeShader::~ComputeShader()
{
	S_RELEASE(this->computeShader);
}

void ComputeShader::init(const std::string& fileName,
	unsigned int threadGroupX, unsigned int threadGroupY, unsigned int threadGroupZ)
{
	this->setThreadGroup(threadGroupX, threadGroupY, threadGroupZ);

	this->createFromFile(fileName);
}

void ComputeShader::run()
{
	// Make sure atleast 1 resource is attached
	if (this->unorderedAccessViews.size() <= 0 && this->renderTextures.size() <= 0 &&
		this->constantBuffers.size() <= 0)
	{
		Log::error("This compute shader has no resources attached to it...");
	}

	// Set
	ID3D11DeviceContext* deviceContext = this->getRenderer().getDeviceContext();
	deviceContext->CSSetShader(this->computeShader, NULL, 0);
	deviceContext->CSSetUnorderedAccessViews(
		0, (UINT)this->unorderedAccessViews.size(), VECTOR_ADDRESS(this->unorderedAccessViews), NULL
	);
	deviceContext->CSSetShaderResources(
		0, (UINT)this->shaderResourceViews.size(), VECTOR_ADDRESS(this->shaderResourceViews)
	);
	deviceContext->CSSetConstantBuffers(
		0, (UINT)this->constantBuffers.size(), VECTOR_ADDRESS(this->constantBuffers)
	);

	// Run
	deviceContext->Dispatch(
		this->threadGroupX,
		this->threadGroupY,
		this->threadGroupZ
	);

	// Reset
	deviceContext->CSSetShader(NULL, NULL, 0);
	deviceContext->CSSetUnorderedAccessViews(0, this->NUM_MAX_UAV, this->uavNULL, NULL);
	deviceContext->CSSetShaderResources(0, this->NUM_MAX_SRV, this->srvNULL);
	deviceContext->CSSetConstantBuffers(0, this->NUM_MAX_CONSTANT_BUFFERS, this->constantBufferNULL);

	// Recreate SRVs after dispatch
	for (size_t i = 0; i < this->renderTextures.size(); ++i)
		this->renderTextures[i]->createShaderResourceView();
}

void ComputeShader::setThreadGroup(
	unsigned int threadGroupX, unsigned int threadGroupY, unsigned int threadGroupZ)
{
	this->threadGroupX = threadGroupX;
	this->threadGroupY = threadGroupY;
	this->threadGroupZ = threadGroupZ;
}

void ComputeShader::setNumConstantBuffers(unsigned int num)
{
	this->constantBuffers.resize(num, nullptr);
}

void ComputeShader::setNumUAVs(unsigned int num)
{
	this->unorderedAccessViews.resize(num, nullptr);
}

void ComputeShader::setNumSRVs(unsigned int num)
{
	this->shaderResourceViews.resize(num, nullptr);
}

void ComputeShader::setNumRenderTextures(unsigned int num)
{
	this->renderTextures.resize(num, nullptr);
	this->setNumUAVs(num);
}

void ComputeShader::addConstantBuffer(ConstantBuffer& buffer, bool allowNullptr)
{
	// Add constant buffer, if we haven't reached the maximum yet
	if (this->constantBuffers.size() < NUM_MAX_CONSTANT_BUFFERS)
	{
		// See if buffer is nullptr
		if (buffer.getBuffer() == nullptr && !allowNullptr)
			Log::error("Added constant buffer to compute shader was nullptr: " + buffer.getDebugName());

		this->constantBuffers.push_back(buffer.getBuffer());
	}
	else
		Log::error("Too many constant buffers were attempted to be added for " + this->debugName);
}

void ComputeShader::addUAV(UAV& uavToAdd, bool allowNullptr)
{
	if (this->unorderedAccessViews.size() < NUM_MAX_UAV)
	{
		// See if UAV is nullptr
		if (uavToAdd.getUAVObject() == nullptr && !allowNullptr)
			Log::error("Added UAV to compute shader is nullptr: " + uavToAdd.getDebugName());

		this->unorderedAccessViews.push_back(uavToAdd.getUAVObject());
	}
	else
		Log::error("Too many UAVs were attempted to be added for " + this->debugName);
}

void ComputeShader::addSRV(SRV& srvToAdd, bool allowNullptr)
{
	if (this->shaderResourceViews.size() < NUM_MAX_SRV)
	{
		// See if SRV is nullptr
		if (srvToAdd.getPtr() == nullptr && !allowNullptr)
			Log::error("Added SRV to compute shader was nullptr: " + srvToAdd.getDebugName());

		this->shaderResourceViews.push_back(srvToAdd.getPtr());
	}
	else
		Log::error("Too many SRVs were attempted to be added for " + this->debugName);
}

/*void ComputeShader::addRenderTexture(Texture& texture)
{
	// Add render texture, if we haven't reached the maximum yet
	if (this->renderTextures.size() < NUM_MAX_RENDER_TEXTURES)
	{
		this->renderTextures.push_back(&texture);
		this->addUAV(texture.getUAV());
	}
	else
		Log::error("Too many render textures were attempted to be added for " + this->debugName);
}*/

void ComputeShader::setConstantBuffer(unsigned int index, ConstantBuffer& bufferToSet)
{
	this->constantBuffers[index] = bufferToSet.getBuffer();
}

void ComputeShader::setUAV(unsigned int index, UAV& uavToSet)
{
	this->unorderedAccessViews[index] = uavToSet.getUAVObject();
}

void ComputeShader::setSRV(unsigned int index, SRV& srvToSet)
{
	this->shaderResourceViews[index] = srvToSet.getPtr();
}

/*void ComputeShader::setRenderTextures(unsigned int index, Texture& textureToSet)
{
	this->renderTextures[index] = &textureToSet;
	this->setUAV(index, textureToSet.getUAV());
}*/