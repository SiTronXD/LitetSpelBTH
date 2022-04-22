#pragma once

#include <string>
#include <vector>

class Str
{
private:

public:
	static void splitString(std::string& str, const char& delim,
		std::vector<std::string>& output);
};