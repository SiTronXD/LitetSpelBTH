#pragma once

#include <string>
#include "../../Engine/Graphics/MeshData.h"

class LevelLoader
{
private:
	MeshData meshData;

public:
	LevelLoader();
	virtual ~LevelLoader();

	bool load(const std::string& levelName);

	inline MeshData& getMeshData() { return this->meshData; }
};
