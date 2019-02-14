#pragma once
#include <algorithm>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <pugixml-1.9/src/pugixml.hpp>
namespace ktexlib
{
	namespace AtlasGen
	{
		typedef std::vector<std::vector<char>> imgvec;
		typedef long double ldb;
		struct bbox//boundries box
		{
			double x, y = 0;
			unsigned short w, h = 0;
		};
		void Atlas(imgvec imgs, std::string output, unsigned int max_altas_size = 2048,unsigned short scale_factor=1);//鸽了，官方python的Atlas()甚至能生成模型
		void AtlasDocumentGen(std::wstring filename, unsigned short imgsize[2],bbox, double offset = 0.5);
	}

}