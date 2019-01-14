// AutoPNG_win32.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include <filesystem>
#include "windows.h"
//可能的注册表项: HKEY_CURRENT_USER\System\GameConfigStore\Children\2c1ae850-e27e-4f10-a985-2dd951d15ba4
//
int main()
{
	using namespace std;
	using namespace filesystem;
	DWORD buffiersize = MAX_PATH;
	wchar_t GameBinPath[MAX_PATH]{ 0 };
	RegGetValueW(HKEY_CURRENT_USER, L"System\\GameConfigStore\\Children\\2c1ae850-e27e-4f10-a985-2dd951d15ba4", L"MatchedExeFullPath",
		RRF_RT_ANY, NULL, GameBinPath, &buffiersize);
	wstring gamebin(GameBinPath);
	wstring modsdir = gamebin + L"../" + L"mods";
	path mods(modsdir);
	vector<string> KTEXpaths(40);
	for (auto &遍历器 : directory_iterator(mods)) 
	{
		auto 内容 = 遍历器.path();//相信我，这玩意能跑
		KTEXpaths.push_back(内容.string());
	}
	for (string filename : KTEXpaths)
	{
		//TODO:转换操作
	}
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
