#pragma once
//已经相当于祖传代码了，年代久远
#include <string>
#include <fstream>
#include <vector>
#include <iostream>

#include <filesystem>
#include <exception>
//多线程控制台输出
#ifdef MULTI_THREAD_KTEXCONOUTPUT
#include <mutex>
#endif
namespace ktexlib
{
	namespace KTEXFileOperation
	{
		struct  //platform
		{
			char opengl = 12;
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

		class KTEXexception :std::exception
		{
		public:
			~KTEXexception() noexcept
			{
				delete[] msg;
			}
			KTEXexception(const char* msg ,int code) noexcept
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
			unsigned int firstblock = 0xFFF00000;
			//0xFFF 12bit, flags 2bit, mipscount 5bit, textype 4bit
			//pixelformat 5bit, platform 4bit
		};
		struct KTEXInfo
		{
			unsigned char flags = 0;
			unsigned short mipscount = 0;
			unsigned char texturetype = textyp.d1;
			unsigned char pixelformat = pixfrm.DXT5;
			unsigned char platform = platfrm.opengl;
		};

		struct mipmap
		{
			unsigned short width = 0;
			unsigned short height = 0;
			unsigned short Z = 1;//Z Axis
			uc_vector data;
		};

		struct RGBAv2
		{
			unsigned short width;
			unsigned short height;
			uc_vector data;
		};

		struct mipmapv2
		{
			unsigned short x;
			unsigned short y;
			unsigned short z = 0;
			unsigned int size = 0;
			char* data = nullptr;
			~mipmapv2();
		};
		
		typedef std::vector<mipmapv2> mipmaps;
		typedef std::vector<RGBAv2> imgs;
		class KTEXFile
		{
		public:
			bool ConvertFromPNG();
			void __fastcall LoadPNG(std::experimental::filesystem::path InputPngFileName, std::string output = "");//使用lodepng 
			bool LoadKTEX(std::wstring FileName);
			void GetRBGAImage(uc_vector& ret);
			mipmap Getmipmapv1();
			KTEXFile();
			~KTEXFile();

			std::wstring output;//输出文件位置
			KTEXHeader Header;
			KTEXInfo Info;
		private:
			size_t __fastcall KTEXMipmapGen(mipmap& target, uc_vector argb_image, unsigned short width, unsigned short height, unsigned short pitch);
			void KTEXFirstBlockGen();
			void __fastcall multimipmapgen(KTEXFileOperation::mipmaps mipmaps, imgs inputimgs);

			std::fstream filestream;

			imgs vec_imgs;
			uc_vector vec_rgba;
			uc_vector vecPNG;

			mipmap mipmap;
			mipmaps vec_mipmaps;
		};
		class KTEXv2
		{
		public:
			void PushRGBA(RGBAv2 RGBA_array);
			void Convert();
			void LoadKTEX(std::experimental::filesystem::path filepath);
			mipmapv2 GetMipmap(unsigned int serial);
			void clear();
			KTEXv2();
			~KTEXv2();

			void operator+=(RGBAv2 src);

			KTEXInfo Info;
			std::wstring output;
		private:
			mipmaps mipmaps;
			KTEXHeader Header;
			imgs RGBA_vectors;
		};
		void operator+(KTEXv2 dest, RGBAv2 src);
	}
}