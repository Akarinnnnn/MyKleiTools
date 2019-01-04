#include "pch.h"
#include "TEXFileOperation.h"

using namespace std;

KTEXFileOperation::KTEXFile::KTEXFile()
{
	
}
KTEXFileOperation::KTEXFile::KTEXFile(wstring InputFileName)
{
	ifsTEX.open(InputFileName,ios::binary);
	bool success=true;
	if (ifsTEX.bad())
	{
		success = false;
		wcout << L"文件损坏/读取失败" << endl;
	}

	unsigned int inputHeader;
	//fsTEX.seekg(0);
	ifsTEX.read((char*)(&inputHeader), 4);//注意括号，我被坑了几次
	inputHeader >>= 16;
	if (inputHeader != Header.ktexheader)
	{
		success = false;
		wcout << L"不是Klei TEX" << endl;
	}





	if(success)
		wcout << L"KTEXFileOperation::KTEXFile " << InputFileName << L" 完成";
	else
		wcout << L"KTEXFileOperation::KTEXFile " << InputFileName << L" 失败";
}
KTEXFileOperation::KTEXFile::~KTEXFile()
{
	ifsTEX.close();
	ofsTEX.close();
}
