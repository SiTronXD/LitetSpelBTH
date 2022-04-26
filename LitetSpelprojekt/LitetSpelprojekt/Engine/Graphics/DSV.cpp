#include "DSV.h"
#include "../Dev/Helpers.h"
#include "Renderer.h"

DSV::DSV(const std::string& debugName)
	: bufferDSV(nullptr), renderer(nullptr), debugName(debugName)
{
}

DSV::~DSV()
{
	S_RELEASE(this->bufferDSV);
}

void DSV::clear()
{
	this->renderer->getDeviceContext()->ClearDepthStencilView(
		this->bufferDSV, D3D11_CLEAR_DEPTH, 1, 0
	);
}

bool DSV::createDSV(
	Renderer& renderer, ID3D11Resource* buffer, 
	const DXGI_FORMAT& format)
{
	this->renderer = &renderer;

	// Deallocate old view
	S_RELEASE(this->bufferDSV);

	// Description
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc{};
	dsvDesc.Format = format;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice = 0;

	// Create DSV
	HRESULT result = this->renderer->getDevice()->CreateDepthStencilView(
		buffer, &dsvDesc, &this->bufferDSV
	);
	if (FAILED(result))
	{
		Log::error("Failed to create DSV.");

		return false;
	}

	return true;
}
