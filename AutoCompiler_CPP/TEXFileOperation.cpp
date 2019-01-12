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

inline void KTEXFileOperation::KTEXFile::KTEXFirstBlockGen()
{
	constexpr unsigned int head = 0x5845544B;

	KTEXHeader tempHeader = this->Header;

	tempHeader.platform = head & 0xF;
	tempHeader.pixelformat = (head >> 4) & 0x1F;
	tempHeader.texturetype = (head >> 9) & 0xF;
	tempHeader.mips = (head >> 13) & 0x1F;
	tempHeader.flags = (head >> 18) & 3;
	//tempHeader.remainder = (head >> 20) & 0xFFF;

	tempHeader.flags <<= 18;
	tempHeader.mips <<= 13;
	tempHeader.texturetype <<= 9;
	tempHeader.pixelformat <<= 4;
	//tempHeader.platform<<=0;
	
	unsigned int* p = &tempHeader.flags;
	for(char i=0;i<=4;i++ )
		Header.firstblock |= *(p+i);
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

	unsigned char* imgdata=nullptr;

	imgdata = image.data();
	
	ofstream ofstex(output,ios::binary|ios::trunc);
	if(!ofstex.is_open())
		_NOT_OPEN;

	KTEXFirstBlockGen();

	ofstex.write("KTEX", 4);
	ofstex.write((char*)(&Header.firstblock), 4);//小端字节序
	
	//单mipmap,想了想gei慌好像用不到一个以上的mipmap

	mipmap = { (unsigned short)wide,(unsigned short)height,0 };

	//写入mipmap信息
	ofstex.write((char*)(&mipmap.width), 2);
	ofstex.write((char*)(&mipmap.height), 2);
	ofstex.write((char*)(&mipmap.Z), 2);

	
	switch(Header.pixelformat)//像素格式判断，压缩，写入mipmap数据
	{ 
		using namespace squish;
		int blockcount = 0;

		case (pixfrm.ARGB):
			mipmap.pdata = (char*)imgdata;
			ofstex.write((char*)mipmap.pdata, wide*height*4);
			break;

		case (pixfrm.DXT1):
			blockcount = GetStorageRequirements(wide, height, kDxt1);
			mipmap.pdata = new char[blockcount];
			Compress(image.data(), mipmap.pdata, kDxt1);
			ofstex.write((char*)mipmap.pdata, blockcount);
			break;

		case (pixfrm.DXT3):
			blockcount = GetStorageRequirements(wide, height, kDxt3);
			mipmap.pdata = new unsigned short[blockcount];
			Compress(image.data(), mipmap.pdata, kDxt3);
			ofstex.write((char*)mipmap.pdata, blockcount*2);
			break;

		case (pixfrm.DXT5):
			blockcount = GetStorageRequirements(wide, height, kDxt5);
			mipmap.pdata = new unsigned short[blockcount];
			Compress(image.data(), mipmap.pdata, kDxt5);
			ofstex.write((char*)mipmap.pdata, blockcount*2);
			break;
	}
	
	
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
