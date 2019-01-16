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
	Header.firstblock = 0;
	mipmap.pdata = nullptr;
}

KTEXFileOperation::KTEXFile::~KTEXFile()
{
	fsTEX.close();
	delete mipmap.pdata;
}

void __fastcall KTEXFileOperation::KTEXFile::multimipmapgen(mipmap_vector inputmip,imgs inputimg)
{
	using namespace KTEXFileOperation;
	auto iter = inputmip.begin();
	auto iterimgs = inputimg.begin();

}

int __fastcall KTEXFileOperation::KTEXFile::KTEXMipmapGen(KTEXFileOperation::mipmap& target,uc_vector image,unsigned short wide,
												unsigned short height,unsigned short Z)
{
	
	int blockcount = 0;
	target.width = wide;
	target.height = height;
	target.Z = Z;
	unsigned char* imgdata = image.data();
	switch (Header.pixelformat)//像素格式判断，压缩，写入mipmap数据
	{
		using namespace squish;
	case (pixfrm.ARGB):
		*target.pdata = image;
		return image.size();
		break;
	case (pixfrm.DXT1):
		target.pdata->resize(GetStorageRequirements(wide, height, kDxt1));
		CompressImage(image.data(), wide, height, target.pdata->data(), kDxt1);
		return target.pdata->size();
		break;
	case (pixfrm.DXT3):
		target.pdata->resize(GetStorageRequirements(wide, height, kDxt3));
		CompressImage(image.data(), wide, height, target.pdata->data(), kDxt3);
		return target.pdata->size();
		break;
	case (pixfrm.DXT5):
		target.pdata->resize(GetStorageRequirements(wide, height, kDxt5));
		CompressImage(image.data(), wide, height, target.pdata->data(), kDxt5);
		return target.pdata->size();
		break;
	default:
		_UNKPIXEL;
	}
	
}

inline void KTEXFileOperation::KTEXFile::KTEXFirstBlockGen()
{
	//constexpr unsigned int head = 0x5845544B;
	unsigned int firstblock = 0;

	firstblock |= 0xFFF;//自己写的有bug，干脆复制粘贴
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
	mipmap.pdata = new uc_vector;
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

	unsigned int datasize = KTEXMipmapGen(mipmap, image, wide, height, 0);

	//写入mipmap信息
	ofstex.write((char*)(&mipmap.width), 2);
	ofstex.write((char*)(&mipmap.height), 2);
	ofstex.write((char*)(&mipmap.Z), 2);
	ofstex.write((char*)(&datasize), 4);
	ofstex.write((char*)mipmap.pdata->data(), mipmap.pdata->size());

	ofstex.close();
	return true;
}

int __fastcall KTEXFileOperation::KTEXFile::LoadPNG(const char* Input)
{
	return lodepng::load_file(this->vecPNG, Input);
}
