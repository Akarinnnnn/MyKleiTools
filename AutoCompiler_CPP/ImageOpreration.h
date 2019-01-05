#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <exception>
namespace ImageOperation
{
	struct PNGsize
	{
		unsigned int width;
		unsigned int height;
	};
	bool ExportImage(std::wstring InputFileName, PNGsize& ret);
}
