#pragma once
#include "basetsd.h"
#include <string>
#include <fstream>
#include <list>
#include <iostream>
namespace KTEXFileOperation
{ 
	struct KTEXHeader
	{
		const unsigned int ktexheader = 0x4B544558;//文件头,老实用ACSII值
		//数据
		unsigned int platform = (unsigned int)platfrm::pc;
		unsigned int pixelformat = (unsigned int)pixfrm::DXT3;
		unsigned int texturetype = (unsigned int)textyp::d2;
		unsigned int mipscount;
		unsigned int flags;
		//unsigned int remainder;
		//end
		//第一数据块
		unsigned int firstblock;//结构见图KTEX_HEADER.bmp(灵魂画技警告)
		//0x3FF 12bit, flags 2bit, mipscount 5bit, textype 4byte
		//pixelformat 5bit, platform 4bit
	};

	struct mipmap
	{
		unsigned short width;//X axis
		unsigned short height;//Y axis
		unsigned short Z;
		iostream* data;
	};
	
	enum class platfrm:char //platform
	{
		pc=12,
		xb360=11,
		ps3=10,
		unk=0
	};
	enum class pixfrm:char //pixel form
	{
		ARGB = 4,
		DXT1 = 0,
		DXT3 = 1,
		DXT5 = 2,
		unk = 7
	};
	enum class textyp:char //texture type
	{
		d1 = 1,//1d
		d2 = 2,//2d
		d3 = 3,//3d
		cube = 4//cubemap
	};


	class KTEXFile
	{
	public:


		KTEXFile(std::wstring InputFileName);
		KTEXFile();
		~KTEXFile();
	private:
		std::ifstream ifsTEX;
		std::ofstream ofsTEX;
		KTEXHeader Header;
	};
}



