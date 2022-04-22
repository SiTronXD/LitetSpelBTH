#include "CubeMap.h"

CubeMap::CubeMap(Renderer& renderer)
	:texture(renderer)
{
}

CubeMap::~CubeMap()
{
}

void CubeMap::createCubeMap(std::string fileName, std::string format)
{
	this->texture.createCubemap(fileName, format);
}
