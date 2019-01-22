// AutoPNG_win32.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include <regex>
#include <filesystem>//MSVC特色，G++自己改一下
#include <thread>
#include <exception>
#include <system_error>
#include <mutex>
#include "..\ktexlib\TEXFileOperation.h"
#include "windows.h"

#ifdef _WIN32
#define MACROSETLOCALE setlocale(LC_ALL, "Chinese_People's Republic of China.936")
#else
#define MACROSETLOCALE setlocale(LC_ALL, "zh_CN.GBK")
#endif
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
			mutex.unlock();
			return true;
		}
		pngfile = *(str.end()-1);
		str.pop_back();
		mutex.unlock();
	}
	else
	{
		return false;
	}

	ktexlib::KTEXFileOperation::KTEXFile KTEX;
	KTEX.LoadPNG(pngfile);
	KTEX.ConvertFromPNG();
	return false;
}

int main()
 {
	MACROSETLOCALE;
	////////////////////这里也改一下，改成std::experemental:filesystem/////////////////////////
	using namespace std::filesystem;
	unsigned long buffiersize = MAX_PATH;
	regex PNGsuffix("(.*)(.png)", regex_constants::icase);
	wchar_t GameBinPath[MAX_PATH]{ 0 };
	RegGetValueW(HKEY_CURRENT_USER, L"System\\GameConfigStore\\Children\\2c1ae850-e27e-4f10-a985-2dd951d15ba4", L"MatchedExeFullPath",
		RRF_RT_ANY, NULL, GameBinPath, &buffiersize);
	wstring modspath(GameBinPath);
	modspath += L"\\..\\..\\mods\\";
	path mods(modspath);
	char num_of_proc[4]{ 0 };
	unsigned char 处理器数量 = 0;
	if (GetEnvironmentVariableA("NUMBER_OF_PROCESSORS", num_of_proc, 4))
	{
		处理器数量 = atoi(num_of_proc); 
	}
	else
	{
		cout << "获取处理器数量失败，转换线程将只有一个" << endl;
		处理器数量 = 1;
	}
	vector<string> PNGs;
	PNGs.reserve(40);
	bool all_clear = false;
	cout << "开始遍历文件" << endl;
	for (auto dir : directory_iterator(mods))
	{
		if (dir.is_directory())
		{
			try
			{
				auto images = canonical(dir.path()) / "images";
				for (auto entries : recursive_directory_iterator(images))
				{
					if (entries.is_regular_file() &&
						regex_match(entries.path().filename().string(), PNGsuffix)
						)
					{
						PNGs.push_back(entries.path().string());
					}
				}
			}
			catch (std::filesystem::filesystem_error e)
			{
				int errcode = e.code().value();
				if(errcode == 3)
				{ }
				else
				{
					cerr << e.what() << endl;
				}
			}
			catch (system_error e)
			{
				cerr << e.what() << endl;
				if (e.code().value() == 1113)
				{
					cout << "文件/文件夹名乱码，这是不行的" << endl;

					cout << "这是大概的名字，搜索出来改个名字或者删掉吧" << endl;
				}
			}
			catch (exception e)
			{
				cerr << e.what() << endl;
			}
			catch (...)
			{
				terminate();
			}
		}

	}
	//std::thread converters[16];//上限16线程
	vector<bool> converter_status(处理器数量);
	
	cout << "开始转换" << endl;
	while (!all_clear)
	{
		all_clear = convert_func(PNGs);
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
