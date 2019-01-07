#pragma once
#include "basetsd.h"
#include <string>
#include <fstream>
#include <list>
#include <vector>
#include <iostream>
#include "lodepng.h"
#include "squish.h"


namespace KTEXFileOperation
{ 
	struct  //platform
	{
		char pc = 12;
		char xb360 = 11;
		char ps3 = 10;
		char unk = 0;
	}constexpr platfrm;
	struct  //pixel form
	{
		char ARGB = 4;
		char DXT1 = 0;
		char DXT3 = 1;
		char DXT5 = 2;
		char unk = 7;
	}constexpr pixfrm;
	struct  //texture type
	{
		char d1 = 1;//1d
		char d2 = 2;//2d
		char d3 = 3;//3d
		char cube = 4;//cubemap
	}constexpr textyp;
	typedef std::vector<unsigned char> uc_vector;
	struct KTEXHeader
	{
		const char ktexheader[4] = { 0x4B,0x54,0x45,0x58 };//文件头,"KTEX",用ACSII值表示
		//数据
		unsigned int flags = 0;
		unsigned int mipscount = 0;
		unsigned int texturetype = (unsigned int)textyp.d2;
		unsigned int pixelformat = (unsigned int)pixfrm.DXT3;
		unsigned int platform = (unsigned int)platfrm.pc;
		unsigned int remainder = 0;
		//unsigned int remainder;
		//end
		//第一数据块
		unsigned int firstblock=0xFFF00000;//结构见图KTEX_HEADER.bmp(灵魂画技警告)
		//0x3FF 12bit, flags 2bit, mipscount 5bit, textype 4byte
		//pixelformat 5bit, platform 4bit
	};

	struct _mipmap
	{
		unsigned short width;//X axis
		unsigned short height;//Y axis
		unsigned short Z=0;
		std::iostream* data;
	};

	struct PNGPIXEL//a,r,g,b
	{
		unsigned char
			*A,
			*R,
			*G,
			*B;
	};

	class KTEXFile
	{
	public:

		bool ConvertFromPNG(uc_vector inputvec);//Warning: This function will clear fsTEX
		

		KTEXFile(std::string InputFileName);
		KTEXFile();
		~KTEXFile();

		std::string output;
	private:
		void KTEXFirstBlockGen();
		std::fstream fsTEX;
		KTEXHeader Header;
		uc_vector vecPNG;
		_mipmap mipmap;
	};
	void movepixel(PNGPIXEL& p);
	void movepixel(PNGPIXEL& p,int count);
}



