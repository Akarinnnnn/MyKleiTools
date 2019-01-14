// AutoCompiler_CPP.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//required lodepng

#include "pch.h"
#include <iostream>
#include <filesystem>

//#define KTEXEXCEPTION
#include "TEXFileOperation.h"

using namespace std;
using namespace KTEXFileOperation;


int main(int argc, char* argv[])
{
	if (MACROSETLOCALE)//CodePage:936
	{
		cout << "Warning: failed to set the codepage to 936, \nconsole outputs may garbled" << endl;
	}
	string input;

	KTEXFile ktex;
	
	
	cout << "input:" << endl;
	cin >> input;
	//filesystem::path Path(input);
	cout << "output:" << endl;
	cin >> ktex.output;
	ktex.LoadPNG(input);
	ktex.ConvertFromPNG();
	
}


/*
int main(int argc,char* argv[])
{
	//string filename;

	//cout << "输入文件名:" << endl;
	//cin >> filename;
	
	//KTEXFile tex(filename);
	unsigned int a = 0x12345678;
	unsigned int b = 0;
	char c[4] = { 0x12,0x34,0x56,0x78 };
	char d[4]{ 0 };
	for (char i = 0; i < 4; i++)
	{
		*((char*)(&b) + i) = *(c + i);//AMD64机 小端字节序
		//顺序：
		//b 00 00 00 00
		//b 00 00 00 12
		//b 00 00 34 12
		//b 00 56 34 12
		//b 78 56 34 12
		*(d + i) = *(c + i);
	}
	for (char i = 0; i < 4; i++)
	*((char*)(&b)+i)=*((char*)(&a)+i);
	
}*/

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
