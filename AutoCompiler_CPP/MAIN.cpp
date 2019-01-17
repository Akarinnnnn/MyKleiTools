// AutoCompiler_CPP.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//required lodepng

#include "pch.h"
#include <iostream>
#include <filesystem>

//#define KTEXEXCEPTION

#include "TEXFileOperation.h"

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
	KTEXFile ktex;
	
	if (argc == 1)
	{
		cout << "No PNG File selected" << endl;
		cout << "Usage: mypng PNG KTEX(Optional)" << endl;
		return 100;
	}
	cout << "input: " << argv[1] << endl;
	if (ktex.LoadPNG(argv[1]) != 0)
		return 101;
	if (argc >= 3)
	{
		ktex.output = argv[2];
	}
	else
	{
		ktex.output = argv[1];
		auto iter = ktex.output.end();
		*(iter - 1) = 'x';
		*(iter - 2) = 'e';
		*(iter - 3) = 't';
	}
	cout << "output: " << ktex.output << endl;
	ktex.ConvertFromPNG();
	return 0;

}
#else
int main()
{
	vector<int> vecint;
	vecint.reserve(10);
	int* vecintdata = vecint.data();
	
	for (int i=0; i < 8; i++)
	{
		*(vecintdata + i) = i;
	}
	cout << ios::oct << vecint.size() << endl;
	return 0;
}
#endif // FUNCTEST
