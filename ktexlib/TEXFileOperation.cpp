//#include "pch.h"

#include <lodepng/lodepng.h>
#include <squish/squish.h>
#include "AltasGen.h"


#include "TEXFileOperation.h"

template<typename T>
inline bool ArrayCompare(T array1[], const T array2[], unsigned long long count)
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

struct mipmapinfile
{
	unsigned short x = 0;
	unsigned short y = 0;
	unsigned short z = 0;
	unsigned int size = 0;
};

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

ktexlib::KTEXFileOperation::KTEXFile::KTEXFile()
{
	wcout.imbue(locale("chs"));
	Header.firstblock = 0;
}

ktexlib::KTEXFileOperation::KTEXFile::~KTEXFile()
{
	filestream.close();
}

void __fastcall ktexlib::KTEXFileOperation::KTEXFile::multimipmapgen(mipmaps inputmip,imgs inputimg)
{
	using namespace ktexlib::KTEXFileOperation;
	auto iter = inputmip.begin();
	auto iterimgs = inputimg.begin();

}

size_t __fastcall ktexlib::KTEXFileOperation::KTEXFile::KTEXMipmapGen(ktexlib::KTEXFileOperation::mipmap& target,uc_vector image,unsigned short wide,
												unsigned short height,unsigned short Z)
{
#ifndef MULTI_THREAD_KTEXCONOUTPUT
	cout << "Generating Mipmap data..." << endl;
#endif
	int blockcount = 0;
	target.width = wide;
	target.height = height;
	target.Z = Z;
	unsigned char* imgdata = image.data();
#ifndef MULTI_THREAD_KTEXCONOUTPUT
	cout << "Compressing..." << endl;
#endif
	switch (Info.pixelformat)//像素格式判断，压缩，写入mipmap数据
	{
		using namespace squish;
	case (pixfrm.ARGB):
		target.data = image;
		return image.size();
		break;
	case (pixfrm.DXT1):
		target.data.resize(GetStorageRequirements(wide, height, kDxt1));
		CompressImage(image.data(), wide, height, target.data.data(), kDxt1);
		return target.data.size();
		break;
	case (pixfrm.DXT3):
		target.data.resize(GetStorageRequirements(wide, height, kDxt3));
		CompressImage(image.data(), wide, height, target.data.data(), kDxt3);
		return target.data.size();
		break;
	case (pixfrm.DXT5):
		target.data.resize(GetStorageRequirements(wide, height, kDxt5));
		CompressImage(image.data(), wide, height, target.data.data(), kDxt5);
		return target.data.size();
		break;
	default:
		throw std::invalid_argument("ktexlib::KTEXFileOperation::KTEXFile::KTEXMipmapGen 像素格式参数错误");
	}
	
}

inline void ktexlib::KTEXFileOperation::KTEXFile::KTEXFirstBlockGen()
{
	//constexpr unsigned int head = 0x5845544B;
#ifndef MULTI_THREAD_KTEXCONOUTPUT
	cout << "Generating KTEXFirstBlock..." << endl;
#endif
	unsigned int firstblock = 0;

	firstblock |= 0xFFF;//自己写的有bug，干脆复制粘贴
	firstblock <<= 2;
	firstblock |= (unsigned int)Info.flags;
	firstblock <<= 5;
	firstblock |= Info.mipscount;
	firstblock <<= 4;
	firstblock |= Info.texturetype;
	firstblock <<= 5;
	firstblock |= Info.pixelformat;
	firstblock <<= 4;
	firstblock |= Info.platform;

	Header.firstblock=firstblock;
}

void mipmapsgen(const ktexlib::KTEXFileOperation::imgs I, ktexlib::KTEXFileOperation::mipmaps& O, ktexlib::KTEXFileOperation::KTEXInfo Info)
{
	using namespace ktexlib::KTEXFileOperation;
	for (auto img : I)
	{
		mipmapv2 temp;
		
		switch (Info.pixelformat)//像素格式判断，压缩，写入mipmap数据
		{
			using namespace squish;
		case (pixfrm.ARGB):
			temp.size = img.size();
			temp.data = (char*)img.data();
			break;
		case (pixfrm.DXT1):
			//target.data.resize(GetStorageRequirements(wide, height, kDxt1));
			temp.size = GetStorageRequirements()
			break;
		case (pixfrm.DXT3):

			break;
		case (pixfrm.DXT5):
			break;
		default:
			throw std::invalid_argument("::mipmapsgen(TEXFileOperation.cpp) 像素格式参数错误");
		}
		O.push_back(temp);
	}
}

bool ktexlib::KTEXFileOperation::KTEXFile::ConvertFromPNG()
{
#ifndef MULTI_THREAD_KTEXCONOUTPUT
	cout << "Convert Start." << endl;
#endif
	using namespace lodepng;
	State imgstate;
	unsigned int wide, height;
	decode(vec_rgba, wide, height, imgstate, this->vecPNG);
	if (wide > USHRT_MAX || height > USHRT_MAX)
	{
		throw std::out_of_range("ktexlib::KTEXFileOperation::KTEXFile::ConvertFromPNG 图片宽/高超过65535");
	}
	
	ofstream ofstex(output,ios::binary|ios::trunc);
	if(!ofstex.is_open())
	{
		cout << "output failure" << endl;
		std::runtime_error("ktexlib::KTEXFileOperation::KTEXFile::ConvertFromPNG 打开失败");
	}
	unsigned int* p_imgvec = (unsigned int*)vec_rgba.data();
	for (unsigned short y = 0; y < height/2; y++)
	{
		auto curline = (unsigned int*)p_imgvec + (y * wide);
		auto tgtline = (unsigned int*)p_imgvec + ((height - y - 1)*wide);
		for (unsigned short x = 0; x < wide; x++)
		{
			unsigned int temp = *(tgtline + x);
			*(tgtline + x) = *(curline + x);
			*(curline + x) = temp;
		}
	}//我只会for循环

	KTEXFirstBlockGen();

	ofstex.write("KTEX", 4);
	ofstex.write((char*)(&Header.firstblock), 4);//小端字节序
	
	//单mipmap,想了想gei慌好像用不到一个以上的mipmap

	unsigned int datasize = KTEXMipmapGen(mipmap, vec_rgba, wide, height, 0);

	//写入mipmap信息
#ifndef MULTI_THREAD_KTEXCONOUTPUT
	cout << "Write mipmap data" << endl;
#endif
	ofstex.write((char*)(&mipmap.width), 2);
	ofstex.write((char*)(&mipmap.height), 2);
	ofstex.write((char*)(&mipmap.Z), 2);
	ofstex.write((char*)(&datasize), 4);
	ofstex.write((char*)mipmap.data.data(), datasize);
	
	ktexlib::AtlasGen::AtlasDocumentGen(output.c_str(), &(mipmap.width), { 0,0,mipmap.width,mipmap.height });

	wcout << output << "\nDone.\n" << endl;

	ofstex.close();
	return true;
}

void __fastcall ktexlib::KTEXFileOperation::KTEXFile::LoadPNG(std::experimental::filesystem::path I,string O)
{
#ifndef MULTI_THREAD_KTEXCONOUTPUT
	cout << "Loading PNG file..." << endl;
	cout << I << endl;
#endif
	output = I.wstring();//wdnm!
	if (O == "")
	{
		output = I.stem().wstring() + L".tex";
	}
	int err = lodepng::load_file(this->vecPNG, I.string());
	if (err == 28)
	{
		cout << "at" + I.string() + ",\n" << lodepng_error_text(28) << endl;
	}
	/*if (err != 0)
	{
		string what = "lodepng error,";
		what += lodepng_error_text(err);
		throw std::exception(what.c_str());
	}*/
}

inline void __fastcall parseheader(ktexlib::KTEXFileOperation::KTEXHeader header, ktexlib::KTEXFileOperation::KTEXInfo& info)
{
	/*
	firstblock |= 0xFFF;//自己写的有bug，干脆复制粘贴
	firstblock <<= 2;
	firstblock |= (unsigned int)Info.flags;
	firstblock <<= 5;
	firstblock |= Info.mipscount;
	firstblock <<= 4;
	firstblock |= Info.texturetype;
	firstblock <<= 5;
	firstblock |= Info.pixelformat;
	firstblock <<= 4;
	firstblock |= Info.platform;
	*/
	//20
	info.flags			 = (header.firstblock & 0x000C0000) >>18;
	info.mipscount		 = (header.firstblock & 0x0003E000) >>13;
	info.texturetype	 = (header.firstblock & 0x00001E00) >>9;
	info.pixelformat	 = (header.firstblock & 0x000001F0) >>4;
	info.platform		 = (header.firstblock & 0x0000000F) ;
}

bool ktexlib::KTEXFileOperation::KTEXFile::LoadKTEX(std::wstring FileName)
{
	size_t filepos = 0;
	ifstream file(FileName);
	mipmapinfile mipinfo;
	uc_vector vec_mipmapdata;
	if (!file.is_open())
		return false;
	file.read((char*)(&Header), 8);
	filepos += 8;
	if (Header.cc4 != 0x5445584B)
		return false;
	parseheader(Header, Info);
	file.seekg(filepos);
	file.read((char*)(&mipinfo), 12);
	filepos += 12;
	file.seekg(filepos);
	vec_mipmapdata.resize(mipinfo.size);
	file.read((char*)vec_mipmapdata.data(), mipinfo.size);
	filepos += mipinfo.size;

	switch (Info.pixelformat)
	{
	case(pixfrm.ARGB):
		vec_rgba = vec_mipmapdata;
	case(pixfrm.DXT5):
		vec_rgba.resize(mipinfo.x*mipinfo.y);
		squish::Decompress(vec_rgba.data(), vec_mipmapdata.data(), squish::kDxt1);
	case(pixfrm.DXT1):
		vec_rgba.resize(mipinfo.x*mipinfo.y);
		squish::Decompress(vec_rgba.data(), vec_mipmapdata.data(), squish::kDxt5);
	case(pixfrm.DXT3):
		vec_rgba.resize(mipinfo.x*mipinfo.y);
		squish::Decompress(vec_rgba.data(), vec_mipmapdata.data(), squish::kDxt3);
	}
	return true;
}

void ktexlib::KTEXFileOperation::KTEXFile::GetRBGAImage(uc_vector & ret)
{
	ret = this->vec_rgba;
}

ktexlib::KTEXFileOperation::mipmap ktexlib::KTEXFileOperation::KTEXFile::Getmipmapv1()
{
	return this->mipmap;
}

void ktexlib::KTEXFileOperation::KTEXv2::PushRGBA(uc_vector RGBA_array)
{
	this->RGBA_vectors.push_back(RGBA_array);
}





void ktexlib::KTEXFileOperation::KTEXv2::Convert()
{

}

void ktexlib::KTEXFileOperation::KTEXv2::LoadKTEX(std::experimental::filesystem::path filepath)
{
	ifstream file(filepath, ios::binary);
	if (!file.is_open())
	{
		throw KTEXexception("failed to open specified file.", -1);
	}
	if (Header.cc4 != 0x5845544B)
	{
		throw KTEXexception("Invalid KTEX file.", 1);
	}
	output = filepath.stem().wstring() + L".tex";
	file.read((char*)(&this->Header), 8);
	parseheader(this->Header, this->Info);
	for (unsigned int i = 0; i < Info.mipscount; i++)
	{
		this->mipmaps.push_back(mipmapv2());
		mipmapv2& target = mipmaps[i];
		file.read((char*)(&target), 10);
		file.seekg(ios::cur, 10);
		auto temp = new char[target.size];
		file.read(temp, target.size);
		file.seekg(ios::cur, target.size);
	}
}

ktexlib::KTEXFileOperation::mipmapv2 ktexlib::KTEXFileOperation::KTEXv2::GetMipmap(unsigned int 序号)
{
	return this->mipmaps[序号];
}

void ktexlib::KTEXFileOperation::KTEXv2::clear()
{
	this->mipmaps.clear();
	this->RGBA_vectors.clear();
	
}

ktexlib::KTEXFileOperation::KTEXv2::~KTEXv2()
{
	//do nothing.
}

void ktexlib::KTEXFileOperation::operator+=(KTEXv2 dest, mipmapv2 src)
{
	dest.mipmaps.push_back(src);
}

void ktexlib::KTEXFileOperation::operator+=(KTEXv2 dest, uc_vector src)
{
	dest.RGBA_vectors.push_back(src);
}

ktexlib::KTEXFileOperation::mipmapv2::~mipmapv2()
{
	delete[] this->data;
}
