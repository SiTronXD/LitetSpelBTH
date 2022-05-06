#include <iostream>
#include <comdef.h>
#include "Log.h"

void Log::writeMat(const DirectX::SimpleMath::Matrix& mat)
{
	Log::write("[" + 
		std::to_string(mat._11) + " " +
		std::to_string(mat._21) + " " +
		std::to_string(mat._31) + " " +
		std::to_string(mat._41) + "]"
	);
	Log::write("[" +
		std::to_string(mat._12) + " " +
		std::to_string(mat._22) + " " +
		std::to_string(mat._32) + " " +
		std::to_string(mat._42) + "]"
	);
	Log::write("[" +
		std::to_string(mat._13) + " " +
		std::to_string(mat._23) + " " +
		std::to_string(mat._33) + " " +
		std::to_string(mat._43) + "]"
	);
	Log::write("[" +
		std::to_string(mat._14) + " " +
		std::to_string(mat._24) + " " +
		std::to_string(mat._34) + " " +
		std::to_string(mat._44) + "]"
	);
	Log::write("");
}

void Log::writeVec(const DirectX::SimpleMath::Vector3& vec)
{
	Log::write("(" + std::to_string(vec.x) + ", " + std::to_string(vec.y) + ", " + std::to_string(vec.z) + ")");
}

void Log::write(const std::string& message)
{
	std::cout << "[Log]: " << message << std::endl;
}

void Log::error(const std::string& errorMessage)
{
	// Simple message box
	MessageBox(
		NULL, errorMessage.c_str(), "ERROR", MB_OK
	);
}

void Log::resultFailed(const std::string& errorMessage, HRESULT& result)
{
	// Create LPCTSTR from HRESULT
	_com_error err(result);
	LPCTSTR errMsg = err.ErrorMessage();

	// Print error together with HRESULT
	Log::error(errorMessage + "\nHRESULT: " + std::string(errMsg));
}