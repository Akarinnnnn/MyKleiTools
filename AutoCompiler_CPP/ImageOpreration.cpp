#include "pch.h"

#include <string>
#include <fstream>
#include <iostream>
#include <iomanip>

#include "ImageOpreration.h"
typedef char byte;
constexpr unsigned long long PNG_HEADER =  0x89504E470D0A1A0A;

using namespace std;
namespace ImageOperation
{
	bool ExportImage(
		std::wstring InputPNGName//png only
	)
	{
		ifstream InputFile(InputPNGName, ios::binary);
		if (!InputFile.good())
		{ 
			wcout << L"文件损坏" << endl;
			return false;
		}
		unsigned long long inputHeader;
		InputFile.read((char*)&inputHeader, 8);
		if (!(inputHeader == PNG_HEADER))
		{
			wcout << L"不是PNG文件头" << endl;
			return false;
		}
		InputFile.seekg(16);
		unsigned short Width=0;
		unsigned short Height=0;

		InputFile.read((byte*)Width, 4);
		InputFile.seekg(4, ios::cur);
		InputFile.read((byte*)Height, 4);
		
		if (!(Width - 1 & Width)&&(Height-1&Height)/*&&(Width==Height)*/)
		{
			wcout << L"长、宽不是2的整数次幂，没法处理" << endl;
			return false;
		}
	}
	bool Convert(wstring sourcefile, wstring destfile)
	{
		return false;
	}
}
