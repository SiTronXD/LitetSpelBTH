#include <comdef.h>
#include "Log.h"

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