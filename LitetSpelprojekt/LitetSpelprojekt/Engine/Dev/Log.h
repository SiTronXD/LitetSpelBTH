#pragma once

#include <string>
#include <Windows.h>
#include <SimpleMath.h>

class Log
{
public:
	static void writeMat(const DirectX::SimpleMath::Matrix& mat);
	static void writeVec(const DirectX::SimpleMath::Vector3& vec);
	static void write(const std::string& message);
	static void warning(const std::string& message);
	static void error(const std::string& errorMessage);
	static void resultFailed(const std::string& errorMessage, HRESULT& result);
};
