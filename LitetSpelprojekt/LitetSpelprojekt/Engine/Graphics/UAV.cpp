#include "UAV.h"
#include "../Dev/Helpers.h"
#include "../Dev/Log.h"
#include "Renderer.h"

UAV::UAV(Renderer& renderer, const std::string& debugName)
	: bufferUAV(nullptr), renderer(renderer), debugName(debugName)
{
}

UAV::~UAV()
{
	S_RELEASE(this->bufferUAV);
}

bool UAV::createTextureUAV(ID3D11Resource* buffer, const DXGI_FORMAT& format)
{
	// Release old UAV
	S_RELEASE(this->bufferUAV);

	// UAV description
	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc{};
	uavDesc.Format = format;
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;

	// Create UAV
	HRESULT result = this->renderer.getDevice()->CreateUnorderedAccessView(
		buffer, &uavDesc, &this->bufferUAV
	);
	if (FAILED(result))
	{
		Log::resultFailed("Failed creating texture UAV: " + this->debugName, result);

		return false;
	}

	return true;
}