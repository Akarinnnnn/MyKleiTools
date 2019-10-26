/****************************
* Encoding: GB2312
****************************/
// AutoCompiler_CPP.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//required lodepng

#include "pch.h"
#include <iostream>
#include <filesystem>
#include "TEXFileOperation.h"
#include "lodepng.h"

//#define FUNCTEST//测试开关
using namespace std;
using namespace ktexlib::KTEXFileOperation;

#ifndef FUNCTEST
int wmain(int argc, const wchar_t* argv[])
{
	wcout.imbue(locale("chs"));
	char input[1024] = { 0 };

	uc_vector File;
	unsigned int w = 0, h = 0;
	unsigned short ws = 0, hs = 0;

	KTEX ktex;
	RGBAv2 img;
	lodepng::State pngstate;
	try 
	{
		switch (argc)
		{
		case(2):
			wcout << L"input: " << argv[1] << endl;
			{
				size_t a = 0;
				wcstombs_s<1024>(&a, input, argv[1], 1000);
			}
			lodepng::load_file(File, input);
			ktex.output = std::filesystem::path(argv[1]).replace_extension(L".tex").wstring();
			break;
		case(3):
			wcout << L"input: " << argv[1] << endl;
			{
				size_t a = 0;
				wcstombs_s<1024>(&a, input, argv[1], 1000);
			}
			lodepng::load_file(File, input);
			ktex.output = wstring(argv[2]);
			break;
		default:
			cout << "Invalid argument.\nUsage: mypng PNG KTEX(Optional)" << endl << "Input filename to contiue" << endl;
			cin >> input;
			ktex.output = std::filesystem::path(input).replace_extension(L".tex").wstring();
			break;
		}
		
	}
	catch (exception e)
	{
		cout << e.what() << endl;
		return 100;
	}

	if (!filesystem::exists(input))
	{
		cerr << "file not exist" << endl;
		return 20;
	}

	wcout << L"output: " << ktex.output << endl;
	lodepng::decode(img.data, w, h, pngstate, File);
	if (pngstate.error != 0)
	{
		cerr << lodepng_error_text(pngstate.error) << endl;
		return pngstate.error;
	}
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
