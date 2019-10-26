/****************************
* Encoding: GB2312
****************************/
#include <iostream>
#include "../ktexlib/TEXFileOperation.h"

int wmain()
{
	using namespace std;
	using namespace ktexlib::KTEXFileOperation;
	wcin.imbue(locale("chs"));
	wcout.imbue(locale("chs"));
	wstring path;
	wcout << L"KTEXÂ·¾¶:" << endl;
	getline(wcin, path);
	KTEX test;
	try
	{
		test.LoadKTEX(path);
	}
	catch (KTEXexception e)
	{
		cout << e.what() << e.code() << endl;
	}
	catch (exception e)
	{
		cout << e.what() << endl;
	}
	catch (...)
	{
		cout << "Î´Öª´íÎó" << endl;
	}
}
