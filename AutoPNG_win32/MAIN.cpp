// AutoPNG_win32.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include <regex>
#include <filesystem>//MSVC特色，G++自己改一下
#include <thread>
#include <atomic>
#include <exception>
#include <system_error>
#include <mutex>
#include "windows.h"

#define MULTI_THREAD_KTEXCONOUTPUT
#include "..\ktexlib\TEXFileOperation.h"


#ifdef _WIN32
#define MACROSETLOCALE setlocale(LC_ALL, "Chinese_People's Republic of China.936")
#else
#define MACROSETLOCALE setlocale(LC_ALL, "zh_CN.GBK")
#endif
//可能的注册表项: HKEY_CURRENT_USER\System\GameConfigStore\Children\2c1ae850-e27e-4f10-a985-2dd951d15ba4
//
using namespace std;

namespace MAIN
{
	std::mutex mutex;
	std::vector<string> PNGs;
}
void convert_func(vector<string>& str,unsigned long long& statuses,unsigned char converterID)
{
	
	string pngfile;
	bool status=false;
	while (!status)
	{
		MAIN::mutex.lock();
		if (str.empty())
		{
			MAIN::mutex.unlock();
			status = true;
			continue;
		}

		pngfile = *(str.end() - 1);
		str.pop_back();
		MAIN::mutex.unlock();

		ktexlib::KTEXFileOperation::KTEXFile KTEX;
		KTEX.LoadPNG(pngfile);
		KTEX.ConvertFromPNG();
	}
	statuses |= (1i64 << converterID);
}

int wmain(int argc,wchar_t* argv[])
 {
	MACROSETLOCALE;
	////////////////////G++这里也改一下，改成std::experimental::filesystem/////////////////////////
	using namespace std::filesystem;
	using namespace MAIN;
	unsigned long buffiersize = MAX_PATH;
	regex PNGsuffix("(.*)(.png)", regex_constants::icase);
	wstring modspath;
	wchar_t GameBinPath[MAX_PATH]{ 0 };
	switch (argc)
	{
	case(1):
		//Linux直接把case(1)去掉
		RegGetValueW(HKEY_CURRENT_USER, L"System\\GameConfigStore\\Children\\2c1ae850-e27e-4f10-a985-2dd951d15ba4", L"MatchedExeFullPath",
			RRF_RT_ANY, NULL, GameBinPath, &buffiersize);
		modspath = GameBinPath;
		modspath += L"\\..\\..\\mods\\";
		break;
	case(2):
		if (*argv[1] == L't' || *argv[1] == L'T')
		{
			modspath = argv[0];
		}
		else
		{
			cout << "输入mods文件夹的路径：" << endl;
			getline(wcin, modspath);
		}
		break;
	default:
		cout << "输入mods文件夹的路径：" << endl;
		getline(wcin, modspath);
		break;
	}

	path mods(modspath);
	/*char num_of_proc[5]{ 0 };
	if (GetEnvironmentVariableA("NUMBER_OF_PROCESSORS", num_of_proc, 4))
	{
		num_of_proc[4] = atoi(num_of_proc); 
	}
	else
	{
		cout << "获取处理器数量失败，转换线程将只有一个" << endl;
		num_of_proc[4] = 1;
	}*/
	PNGs.reserve(40);
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
			catch (std::filesystem::filesystem_error& e)
			{
				int errcode = e.code().value();
				if (errcode == 3)//就是单纯的找不到文件 
				{} 
				else
				{
					cerr << e.what() << endl;
				}
			}
			catch (system_error& e)
			{
				cerr << e.what() << endl;
				if (e.code().value() == 1113)
				{
					cout << "文件/文件夹名乱码" << endl;
				}
			}
			catch (exception& e)
			{
				cerr << e.what() << endl;
			}
			catch (...)
			{
				terminate();
			}
		}

	}
	
	cout << "开始转换" << endl;

	unsigned long long clear_status = 0;
	unsigned long long converter_status = 0;
	auto cpuscount = thread::hardware_concurrency();
	/*for (unsigned char i = 0; i < cpuscount && i<64 ; i++)
	//for(int i=0;i==0;i++)
	{
		clear_status |= (1i64 << i);
		thread converter(convert_func,ref(PNGs), ref(converter_status), i);
		converter.detach();
	}
	//exception_ptr pexcetion();

	//a^b = xor a b，挂机等
	while (converter_status ^ clear_status)
	{
		Sleep(1000);
	}*/

	convert_func(PNGs, converter_status, 0);
	{
		Sleep(500);
	}
	cout << "完成" << endl;

	//在搞多线程
	
}

