#include "pch.h"

#include "basetsd.h"//exists in VC++

#include "ImageOpreration.h"
//wchar_t = 2B(16 bits)
constexpr UINT64 PNG_HEADER =  0x89504E470D0A1A0A;//8Bytes



using namespace std;

namespace ImageOperation
{
	bool ExportImage(
		std::wstring InputPNGName,//png only
		PNGsize& ret
	)
	{
		ifstream InputFile(InputPNGName, ios::binary);
		if (!InputFile.good())
		{
			wcout << L"文件损坏" << endl;
			return false;
		}
		UINT64 inputHeader;
		InputFile.read((char*)(&inputHeader), 8);
		if (inputHeader != PNG_HEADER)
		{
			wcout << L"不是PNG文件头" << endl;
			return false;
		}
		InputFile.seekg(16);
		UINT32 Width = 0;
		UINT32 Height = 0;

		InputFile.read((char*)(&Width), 4);
		InputFile.seekg(2, ios::cur);
		InputFile.read((char*)(&Height), 4);

		if (!(Width - 1 & Width) && (Height - 1 & Height))//is power of 2
		{
			wcout << L"长、宽不是2的整数次幂" << endl;
			return false;
		}
		//TODO: Convert Code
		ret.height = Height;
		ret.width = Width;
		return true;
	}

	bool Convert(std::wstring sourcefile, std::wstring destfile)
	{
		bool generatemipmaps = false;
		bool premutiplyalpha = false;

		return false;
	}
}


template<typename T>
bool ArrayCompare(T array1, T array2,UINT64 length1)//数组长度诚实一点，出bug别怪我
{
	for (UINT64 i=0;; i++)
	{
		if (*(array1+i) != *(array2+i))
			return false;
	}
	return true;
}