#include "CubeMap.h"

CubeMap::CubeMap(Renderer& renderer)
	:texture(renderer)
{
}

CubeMap::~CubeMap()
{
}

void CubeMap::createCubeMap(std::string path)
{
	this->texture.createCubemap(path);
}
