#include "Log.h"

void Log::error(const std::string& errorMessage)
{
	// Simple message box
	MessageBox(
		NULL, errorMessage.c_str(), "ERROR", MB_OK
	);
}