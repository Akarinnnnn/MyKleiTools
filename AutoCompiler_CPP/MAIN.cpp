// AutoCompiler_CPP.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//required lodepng

#include "pch.h"
#include <iostream>
#include <filesystem>



#include "..\ktexlib\TEXFileOperation.h"

//#define FUNCTEST//测试开关

using namespace std;
using namespace KTEXFileOperation;

#ifndef FUNCTEST
int main(int argc, const char* argv[])
{
	if (MACROSETLOCALE)//CodePage:936
	{
		cout << "Warning: failed to set the codepage to 936, \nconsole outputs may garbled" << endl;
	}
	string input;
	KTEXFile ktex;
	try 
	{
		switch (argc)
		{
		case(1):
			cout << "Usage: mypng PNG KTEX(Optional)" << endl << "Input filename to contiue" << endl;
			cin >> input;
			ktex.LoadPNG(input);
			break;
		case(2):
			cout << "input: " << argv[1] << endl;
			ktex.LoadPNG(argv[1]);
		case(3):
			ktex.output = argv[2];
		default:
			break;
		}
	}
	catch (exception e)
	{
		cout << e.what() << endl;
		return 100;
	}
	cout << "output: " << ktex.output << endl;
	ktex.ConvertFromPNG();
	return 0;

}
#else
int main()
{
	auto height = 2;
	auto wide = 2;
	unsigned char p_imgvec[16] =
	{ 
	1,2,3,4,  5,6,7,8,

	9,10,11,12,  13,14,15,16
	};

	for (unsigned short y = 0; y < height; y++)
	{
		auto curline = (unsigned int*)p_imgvec + (y * wide);
		auto tgtline = (unsigned int*)p_imgvec + ((height - y - 1)*wide);
		for (unsigned short x = 0; x < wide; x++)
		{
			unsigned int temp = *(tgtline + x);
			*(tgtline + x) = *(curline + x);
			*(curline + x) = temp;
		}
	}
	return 0;
}
#endif // FUNCTEST
