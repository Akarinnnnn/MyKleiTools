#pragma once
#include <map>
#include <string>

class localization
{
public:
	std::map<uint16_t, std::wstring> table; //k-v table?
	localization();
	std::wstring& operator[](unsigned short i);
};