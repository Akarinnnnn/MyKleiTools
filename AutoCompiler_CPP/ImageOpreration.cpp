#include "pch.h"
#include "ImageOpreration.h"
#include <string>
#include <fstream>
#include <iomanip>



using namespace std;
namespace ImageOperation
{
	bool ExportImage(
		std::wstring InputFileDOSName//png only
	)
	{
		ifstream InputFile(InputFileDOSName, ios::binary);
		if (!InputFile.good())
			return false;
		unsigned long FileByte = InputFile.tellg();
		unsigned short Width;
		unsigned short Height;
		
	}
}
