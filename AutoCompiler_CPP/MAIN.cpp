// AutoCompiler_CPP.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//required lodepng

#include "pch.h"
#include <iostream>
#include <filesystem>
#include "..\ktexlib\TEXFileOperation.h"
#include "lodepng.h"

//#define FUNCTEST//测试开关
using namespace std;
using namespace ktexlib::KTEXFileOperation;

#ifndef FUNCTEST
int wmain(int argc, const wchar_t* argv[])
{
	if (MACROSETLOCALE)//CodePage:936
	{
		cout << "Warning: failed to set the codepage to 936, \nconsole outputs may garbled" << endl;
	}
	string input(argv[1]+0, argv[1]+
		[argv]()
	{
		size_t i = 0;
		while (argv[1][i] != '\0')
			i++;
		return i;
	}());
	uc_vector File;
	unsigned int w, h = 0;
	unsigned short ws, hs = 0;

	KTEX ktex;
	RGBAv2 img;
	lodepng::State pngstate;
	try 
	{
		switch (argc)
		{
		case(2):
			wcout << L"input: " << argv[1] << endl;
			lodepng::load_file(File, input);
			ktex.output = std::filesystem::path(argv[1]).filename().wstring();
			break;
		case(3):
			wcout << L"input: " << argv[1] << endl;
			lodepng::load_file(File, input);
			{
				ktex.output = wstring(argv[2]);
				break;
			}
		default:
			cout << "Invalid argument.\nUsage: mypng PNG KTEX(Optional)" << endl << "Input filename to contiue" << endl;
			cin >> input;
			break;
		}
		
	}
	catch (exception e)
	{
		cout << e.what() << endl;
		return 100;
	}
	wcout << L"output: " << ktex.output << endl;
	lodepng::decode(img.data, w, h, pngstate, File);
	if (w > USHRT_MAX || h > USHRT_MAX)
	{
		return 999;
	}
	else
	{
		hs = (unsigned short)h;
		ws = (unsigned short)w;
	}
	img.height = hs; img.width = ws;
	ktex.PushRGBA(img,1);
	ktex.Convert();
	return 0;
	
}
#else
int main()
{

}
#endif // FUNCTEST
