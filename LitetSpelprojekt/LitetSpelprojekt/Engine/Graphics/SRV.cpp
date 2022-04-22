#include "SRV.h"
#include "../Dev/Helpers.h"
#include "../Dev/Log.h"
#include "Renderer.h"

SRV::SRV(Renderer& renderer, const std::string& debugName)
	: renderer(renderer), debugName(debugName),
	resourceSRV(nullptr)
{
}

SRV::~SRV()
{
	S_RELEASE(this->resourceSRV);
}

bool SRV::createTextureSRV(ID3D11Resource* resource, const DXGI_FORMAT& format, const D3D11_SRV_DIMENSION& dimension)
{
	S_RELEASE(this->resourceSRV);

	// Create texture SRV
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = format;
	srvDesc.ViewDimension = dimension;
	srvDesc.Texture2D.MipLevels = 1;
	srvDesc.Texture2D.MostDetailedMip = 0;

	HRESULT result = this->renderer.getDevice()->CreateShaderResourceView(
		resource, &srvDesc, &this->resourceSRV
	);
	if (FAILED(result))
	{
		Log::resultFailed("Failed creating texture SRV: " + this->debugName, result);

		return false;
	}

	return true;
}
