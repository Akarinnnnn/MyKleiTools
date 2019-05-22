#pragma once
#ifndef KTEX_COMMON
#define KTEX_COMMON

#if KTEXLIBDYMANIC_EXPORTS
#define __API __declspec(dllexport)
#else
#define __API __declspec(dllimport)
#endif

#include <filesystem>
#include <vector>

namespace ktexlib
{
	typedef std::vector<unsigned char> uc_vector;
	typedef std::vector<uc_vector> datavec;
	namespace Atlas
	{
		struct b_box//boundry box
		{
			unsigned short w;	//width
			unsigned short h;	//height
			double x;			//x offset
			double y;			//y offset
		};
	}
	namespace KTEXFileOperation
	{
		enum class  platfrm//platform
		{
			opengl = 12,
			xb360 = 11,
			ps3 = 10,
			unk = 0
		};
		enum class pixfrm //pixel form
		{
			ARGB = 4,
			DXT1 = 0,
			DXT3 = 1,
			DXT5 = 2,
			unk = 7
		};
		enum class textyp //texture type
		{
			d1 = 1,//1d
			d2 = 2,//2d
			d3 = 3,//3d
			cube = 4//cubemap
		};
		class __API KTEXexception :public std::exception
		{
		public:
			~KTEXexception() noexcept
			{
				delete[] msg;
			}
			KTEXexception(const char* msg, int code) noexcept
			{
				this->msg = msg;
				this->_code = code;
			}
			char const* what() const noexcept
			{
				return msg;
			}
			int const code() noexcept
			{
				return _code;
			}
		private:
			char const* msg;
			int _code;
		};

		struct KTEXHeader
		{
			//CC4
			unsigned int cc4 = 0x5845544B;
			//第一数据块
			unsigned int firstblock = 0;
			//0xFFF 12bit, flags 2bit, mipscount 5bit, textype 4bit
			//pixelformat 5bit, platform 4bit
		};
		struct KTEXInfo
		{
			unsigned char flags = 0;
			unsigned short mipscount = 0;
			textyp texturetype = textyp::d1;
			pixfrm pixelformat = pixfrm::DXT5;
			platfrm platform = platfrm::opengl;
		};

		struct RGBAv2
		{
			unsigned short width = 0;
			unsigned short height = 0;
			unsigned short pitch = 0;
			uc_vector data;
		};

		struct mipmapv2
		{
			unsigned short width = 0;
			unsigned short height = 0;
			unsigned short pitch = 0;
			unsigned int size = 0;
			char* data = nullptr;
			__API ~mipmapv2();
		};

		typedef std::vector<mipmapv2> mipmaps;
		typedef std::vector<RGBAv2> imgs;

	}
}
#endif