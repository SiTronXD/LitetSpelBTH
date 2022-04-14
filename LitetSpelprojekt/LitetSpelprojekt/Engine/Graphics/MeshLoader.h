#pragma once

#include <string>
#include "MeshData.h"

class MeshLoader
{
private:
public:
	MeshLoader();
	virtual ~MeshLoader();

	static MeshData loadModel(
		const std::string& fileName, bool flipVerticalUV = true
	);
};