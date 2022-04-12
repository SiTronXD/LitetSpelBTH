#pragma once

#include <string>

class Material
{
private:
	std::string diffuseTextureName;

	std::string vertexShaderName;
	std::string pixelShaderName;

public:
	Material(const std::string& diffuseTextureName);
	virtual ~Material();

	inline const std::string& getDiffuseTextureName() { return this->diffuseTextureName; }

};