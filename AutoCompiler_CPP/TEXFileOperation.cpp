#include "pch.h"
#include "TEXFileOperation.h"

template<typename T>
inline bool ArrayCompare(T array1[4], const T array2[4], unsigned long long count)
{
	//UINT64 p1,p2 =(UINT64*)array1,(UINT64*)array2
	for (unsigned long long i = 0; i < count; i++)
	{
		if (*(array1 + i) != *(array2 + i))
		{
			return false;
		}
	}
	return true;
}

char* ReverseByByte(char* p, unsigned long long bytecount)//p强制转换就行
{
	char mid = 0;
	for (unsigned long long i = 0; i < (bytecount / 2); i++)
	{
		mid = *(p + (bytecount - i - 1));
		*(p + (bytecount - i - 1)) = *(p + i);
		*(p + i) = mid;
	}
	return p;
}

using namespace std;

KTEXFileOperation::KTEXFile::KTEXFile()
{
	
}

/*KTEXFileOperation::KTEXFile::KTEXFile(string InputFileName)
{
	fsTEX.open(InputFileName, ios::in | ios::out | ios::binary);
	cout << "KETXFileOperation::KTEXFile::KTEXFile 载入" << InputFileName << endl;
	bool success=true;
	char inputHeader[4]{0};

	if (fsTEX.fail())
	{
		success = false;
		
#ifdef KTEXEXCEPTION
		throw KTEXexception("读取失败")
#else
		cout << "读取失败" << endl;
		goto failure;
#endif 

	}
	if (fsTEX.bad())
	{
		success = false;
#ifdef KTEXEXCEPTION
		throw KTEXexception("文件损坏")
#else
		cout << "文件损坏" << endl;
		goto failure;
#endif
	}

	fsTEX.read(inputHeader, 4);//注意括号，我被坑了几次
	ReverseByByte((char*)(&inputHeader),4);
	if (inputHeader == Header.ktexheader)
	{
		success = false;
		cout << "不是Klei TEX" << endl;
	}

	if(success)
		cout << "KTEXFileOperation::KTEXFile " << InputFileName << " 完成";
	else
		failure:
		cout << "KTEXFileOperation::KTEXFile " << InputFileName << " 失败";
}//不建议用*/
KTEXFileOperation::KTEXFile::~KTEXFile()
{
	fsTEX.close();
	delete[] mipmap.pdata;
}

int __fastcall KTEXFileOperation::KTEXFile::KTEXMipmapGen(_mipmap& target,uc_vector& image,unsigned short wide,
												unsigned short height,unsigned short Z)
{
	int blockcount = 0;
	target.width = wide;
	target.height = height;
	target.Z = Z;
	unsigned char* imgdata = image.data();//不引用，返回的是拷贝的地址，数据会变成GC后的东西
	switch (Header.pixelformat)//像素格式判断，压缩，写入mipmap数据
	{
		using namespace squish;
	case (pixfrm.ARGB):
		target.pdata = imgdata;
		return blockcount = wide * height * 4;
		break;
	case (pixfrm.DXT1):
		blockcount = GetStorageRequirements(wide, height, kDxt1);
		target.pdata = new char[blockcount];
		Compress(image.data(), target.pdata, kDxt1);
		return blockcount * 2;
		break;

	case (pixfrm.DXT3):
		blockcount = GetStorageRequirements(wide, height, kDxt3);
		target.pdata = new unsigned short[blockcount];
		Compress(image.data(), target.pdata, kDxt3);
		return blockcount * 2;
		break;

	case (pixfrm.DXT5):
		blockcount = GetStorageRequirements(wide, height, kDxt5);
		target.pdata = new unsigned short[blockcount];
		Compress(image.data(), target.pdata, kDxt5);
		return blockcount * 2;
		break;
	default:
		_UNKPIXEL;
	}
	
}

inline void KTEXFileOperation::KTEXFile::KTEXFirstBlockGen()
{
	constexpr unsigned int head = 0x5845544B;
	unsigned int firstblock = 0;

	firstblock |= 4095;//自己写的有bug，干脆复制粘贴
	firstblock <<= 2;
	firstblock |= Header.flags;
	firstblock <<= 5;
	firstblock |= Header.mips;
	firstblock <<= 4;
	firstblock |= Header.texturetype;
	firstblock <<= 5;
	firstblock |= Header.pixelformat;
	firstblock <<= 4;
	firstblock |= Header.platform;

	Header.firstblock=firstblock;
}



bool KTEXFileOperation::KTEXFile::ConvertFromPNG()
{
	using namespace lodepng;
	//namespace fs = std::filesystem;
	uc_vector image;//RGBA
	State imgstate;
	unsigned int wide, height;
	decode(image, wide, height, imgstate, this->vecPNG);

	if (wide > USHRT_MAX || height > USHRT_MAX)
	{
		_WH_OUT_OF_RANGE;
	}
	
	ofstream ofstex(output,ios::binary|ios::trunc);
	if(!ofstex.is_open())
		_NOT_OPEN;

	KTEXFirstBlockGen();

	ofstex.write("KTEX", 4);
	ofstex.write((char*)(&Header.firstblock), 4);//小端字节序
	
	//单mipmap,想了想gei慌好像用不到一个以上的mipmap

	int datasize = KTEXMipmapGen(mipmap, image, wide, height, 0);

	//写入mipmap信息
	ofstex.write((char*)(&mipmap.width), 2);
	ofstex.write((char*)(&mipmap.height), 2);
	ofstex.write((char*)(&mipmap.Z), 2);	
	ofstex.write((char*)mipmap.pdata, datasize);

	ofstex.close();
	return true;
}

int __fastcall KTEXFileOperation::KTEXFile::LoadPNG(std::string Input)
{
	return lodepng::load_file(this->vecPNG, Input);
}

//exception
KTEXFileOperation::KTEXexception::KTEXexception()noexcept
{

}

KTEXFileOperation::KTEXexception::KTEXexception(char * MSG)
{
	this->data.data = MSG;
}

KTEXFileOperation::KTEXexception::~KTEXexception()
{}

const char * KTEXFileOperation::KTEXexception::what()noexcept
{
	return this->data.data;
}

KTEXFileOperation::KTEXexception & KTEXFileOperation::KTEXexception::operator=(KTEXexception a)
{
	if (this == &a)
	{
		return *this;
	}
	this->data.data = a.data.data;
	this->data.dofree = a.data.dofree;

	return *this;
}
