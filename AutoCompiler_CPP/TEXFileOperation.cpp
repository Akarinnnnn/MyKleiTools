#include "pch.h"
#include "TEXFileOperation.h"

using namespace std;

KTEXFileOperation::KTEXFile::KTEXFile()
{
	
}

KTEXFileOperation::KTEXFile::KTEXFile(wstring InputFileName)
{
	fsTEX.open(InputFileName,ios::binary);
	bool success=true;
	if (fsTEX.bad())
	{
		success = false;
		wcout << L"文件损坏/读取失败" << endl;
	}

	unsigned int inputHeader;
	//fsTEX.seekg(0);
	fsTEX.read((char*)(&inputHeader), 4);//注意括号，我被坑了几次
	inputHeader >>= 16;
	if (inputHeader != Header.ktexheader)
	{
		success = false;
		wcout << L"不是Klei TEX" << endl;
	}
	
	

	if(success)
		wcout << L"KTEXFileOperation::KTEXFile " << InputFileName << L" 完成";
	else
		wcout << L"KTEXFileOperation::KTEXFile " << InputFileName << L" 失败";
}
KTEXFileOperation::KTEXFile::~KTEXFile()
{
	fsTEX.close();
}

inline void KTEXFileOperation::KTEXFile::KTEXHeaderGen()
{
	constexpr unsigned int head = 0x4B544558;
	Header.platform = head & 0xF;
	Header.pixelformat = (head >> 4) & 0x1F;
	Header.texturetype = (head >> 9) & 0xF;
	Header.mipscount = (head >> 13) & 0x1F;
	Header.flags = (head >> 18) & 3;
	Header.remainder = (head >> 20) & 0xFFF;
	
}

bool KTEXFileOperation::KTEXFile::ConvertFromPNG(KTEXFileOperation::uc_vector inputvec)
{
	using namespace lodepng;

	uc_vector image;//RGBA
	State imgstate;
	unsigned int wide, height;
	decode(image, wide, height, imgstate, inputvec);
	PNGPIXEL pixel{nullptr,nullptr,nullptr,nullptr};
	image._Make_iterator(pixel.R);

	pixel.G = pixel.R + 1;//考虑弃用
	pixel.B = pixel.R + 2;
	pixel.A = pixel.R + 3;

	fsTEX.clear();
	fsTEX.open(output);
	
	KTEXHeaderGen();

	return false;
}

void KTEXFileOperation::KTEXFile::setoutput(wstring a)
{
	output = a;
}
inline void KTEXFileOperation::movepixel(PNGPIXEL& p)
{
	p.A += 4,
	p.B += 4,
	p.G += 4,
	p.R += 4;
}

inline void KTEXFileOperation::movepixel(PNGPIXEL& p, int count)
{
	
}
