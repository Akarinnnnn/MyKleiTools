#include "pch.h"
#include "TEXFileOperation.h"



template<typename T>
inline bool ArrayCompare(T array1[4], const T array2[4], UINT64 count)
{
	//UINT64 p1,p2 =(UINT64*)array1,(UINT64*)array2
	for (UINT64 i = 0; i < count; i++)
	{
		if (*(array1 + i) != *(array2 + i))
		{
			return false;
		}
	}
	return true;
}

using namespace std;

KTEXFileOperation::KTEXFile::KTEXFile()
{
	
}

KTEXFileOperation::KTEXFile::KTEXFile(string InputFileName)
{
	fsTEX.open(InputFileName, ios::in | ios::out | ios::binary);
	cout << "KETXFileOperation::KTEXFile::KTEXFile 载入" << InputFileName << endl;
	bool success=true;
	char inputHeader[4]{0};

	if (fsTEX.fail())
	{
		success = false;
		cout << "读取失败" << endl;
		goto failure;
	}
	if (fsTEX.bad())
	{
		success = false;
		cout << "文件损坏" << endl;
		goto failure;
	}

	fsTEX.read(inputHeader, 4);//注意括号，我被坑了几次
	
	if (!ArrayCompare(inputHeader,Header.ktexheader,4))
	{
		success = false;
		cout << "不是Klei TEX" << endl;
	}

	if(success)
		cout << "KTEXFileOperation::KTEXFile " << InputFileName << " 完成";
	else
		failure:
		cout << "KTEXFileOperation::KTEXFile " << InputFileName << " 失败";
}
KTEXFileOperation::KTEXFile::~KTEXFile()
{
	fsTEX.close();
}

inline void KTEXFileOperation::KTEXFile::KTEXFirstBlockGen()
{
	constexpr unsigned int head = 0x4B544558;
	Header.platform = head & 0xF;
	Header.pixelformat = (head >> 4) & 0x1F;
	Header.texturetype = (head >> 9) & 0xF;
	Header.mipscount = (head >> 13) & 0x1F;
	Header.flags = (head >> 18) & 3;
	//Header.remainder = (head >> 20) & 0xFFF;

	Header.flags <<= 18;
	Header.mipscount <<= 13;
	Header.texturetype <<= 9;
	Header.pixelformat <<= 4;
	//Header.platform<<=0;
	
	unsigned int* p = &Header.flags;
	for(char i=0;i<=4;i++ )
		Header.firstblock |= *(p+i);
}

bool KTEXFileOperation::KTEXFile::ConvertFromPNG(KTEXFileOperation::uc_vector inputvec)
{
	using namespace lodepng;

	uc_vector image;//RGBA
	uc_vector ret;
	State imgstate;
	unsigned int wide, height;
	decode(image, wide, height, imgstate, inputvec);

	unsigned char* imgvecp=nullptr;
	unsigned char* retvecp=nullptr;
	
	ret.resize(wide*height);

	imgvecp = image.data();
	retvecp = ret.data();
	
	fsTEX.clear();
	fsTEX.open(this->output,ios::binary|ios::trunc);

	if(fsTEX.bad())
		return false;
	
	KTEXFirstBlockGen();

	fsTEX.write((char*)(&(Header.ktexheader)), 4);
	fsTEX.write((char*)(&(Header.firstblock)), 4);
	
	switch(Header.pixelformat)//像素格式
	{ 
		case (pixfrm.ARGB):
			fsTEX.write((char*)image.data(), wide*height);
			break;
		case (pixfrm.DXT1):
			squish::CompressImage(imgvecp, wide, height, retvecp, squish::kDxt1);
			fsTEX.write((char*)ret.data(), wide*height);
			break;
		case (pixfrm.DXT3):
			squish::CompressImage(imgvecp, wide, height, retvecp, squish::kDxt3);
			fsTEX.write((char*)ret.data(), wide*height);
			break;
		case (pixfrm.DXT5):
			squish::CompressImage(imgvecp, wide, height, retvecp, squish::kDxt5);
			fsTEX.write((char*)ret.data(), wide*height);
			break;
	}

	fsTEX.close();
	return true;
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



