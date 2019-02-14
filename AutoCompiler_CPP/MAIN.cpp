// AutoCompiler_CPP.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//required lodepng

#include "pch.h"
#include <iostream>
#include "..\ktexlib\TEXFileOperation.h"

//#define FUNCTEST//测试开关

using namespace std;
using namespace ktexlib::KTEXFileOperation;

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
		case(2):
			cout << "input: " << argv[1] << endl;
			ktex.LoadPNG(argv[1]);
			break;
		case(3):
			cout << "input: " << argv[1] << endl;
			ktex.LoadPNG(argv[1]);
			auto buff = new wchar_t[_MAX_PATH];
			mbstowcs(buff, argv[2], _MAX_PATH);
			ktex.output = buff;
			delete[] buff;
			break;
		default:
			cout << "Invalid argument.\nUsage: mypng PNG KTEX(Optional)" << endl << "Input filename to contiue" << endl;
			cin >> input;
			ktex.LoadPNG(input);
			break;
		}
	}
	catch (exception e)
	{
		cout << e.what() << endl;
		return 100;
	}
	wcout << L"output: " << ktex.output << endl;
	ktex.ConvertFromPNG();
	return 0;

}
#else
int main()
{

}
#endif // FUNCTEST
