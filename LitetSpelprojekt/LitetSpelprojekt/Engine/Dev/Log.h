#pragma once

#include <string>
#include <d3d11.h>

class Log
{
public:
	static void error(const std::string& errorMessage);
};