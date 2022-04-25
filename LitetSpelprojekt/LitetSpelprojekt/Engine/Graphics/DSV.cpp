#include "DSV.h"
#include "../Dev/Helpers.h"

DSV::DSV(Renderer& renderer, const std::string& debugName)
	: bufferDSV(nullptr), renderer(renderer), debugName(debugName)
{
}

DSV::~DSV()
{
	S_RELEASE(this->bufferDSV);
}

void DSV::clear()
{
}

bool DSV::createDSV(ID3D11Resource* buffer, const DXGI_FORMAT& format)
{
	S_RELEASE(this->bufferDSV);

	// Description
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc{};
	dsvDesc.Format = format;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

	return true;
}
