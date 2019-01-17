// AutoPNG_win32.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include <regex>
#include <filesystem>
#include <thread>
#include <exception>
#include <mutex>
#include "..\AutoCompiler_CPP\TEXFileOperation.h"
#include "windows.h"
//可能的注册表项: HKEY_CURRENT_USER\System\GameConfigStore\Children\2c1ae850-e27e-4f10-a985-2dd951d15ba4
//
using namespace std;
bool convert_func(vector<string>& str)
{
	mutex mutex;
	string pngfile;
	if (mutex.try_lock())
	{
		if (str.empty())
		{
			return true;
		}
		pngfile = *str.end();
		str.pop_back();
		mutex.unlock();
	}
	else
	{
		return false;
	}

	KTEXFileOperation::KTEXFile KTEX;
	KTEX.LoadPNG(pngfile);
	KTEX.ConvertFromPNG();
	return false;
}

int main()
{
	using namespace std::filesystem;
	DWORD buffiersize = MAX_PATH;
	regex PNGsuffix(".png", regex_constants::icase);
	//wchar_t GameBinPath[MAX_PATH]{ 0 };
	//RegGetValueW(HKEY_CURRENT_USER, L"System\\GameConfigStore\\Children\\2c1ae850-e27e-4f10-a985-2dd951d15ba4", L"MatchedExeFullPath",
		//RRF_RT_ANY, NULL, GameBinPath, &buffiersize);
	//wstring gamebin(GameBinPath);
	
	path mods("mods");
	vector<string> KTEXpaths(40);
	bool all_clear = false;
try
{
	for (auto &遍历器 : directory_iterator(mods)) 
	{
		auto 内容 = 遍历器.path();//相信我，这玩意能跑
		if (regex_search(内容.u8string(), PNGsuffix))
		{
			KTEXpaths.push_back(内容.stem().u8string());
		}
	}
}
catch(std::filesystem::filesystem_error e)
{
	cerr << e.what() << endl;
}
	std::thread converters[16];//max 16 threads
	while (!all_clear)
	{
		all_clear = convert_func(KTEXpaths);
	}
	//想搞多线程
	
}



// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
