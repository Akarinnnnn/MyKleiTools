// AutoPNG_win32.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include "stringslist_CHS.h"

//STL
#include <iostream>
#include <regex>
#include <filesystem>//MSVC特色
#include <thread>
#include <atomic>
#include <exception>
#include <system_error>
#include <mutex>
//Win32
#ifdef _WIN32
#include "windows.h"//Linux把这个去了
#endif
#define MULTI_THREAD_KTEXCONOUTPUT
#include "..\ktexlib\TEXFileOperation.h"

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
	MACROSETLOCALE;
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
	////////////////////也是MSVC特色/////////////////////////
	using namespace std::filesystem;
	using namespace MAIN;
	unsigned long buffiersize = MAX_PATH;
	regex PNGsuffix("(.*)(.png)", regex_constants::icase);
	wstring modspath;
	wchar_t GameBinPath[MAX_PATH]{ 0 };
	bool 清理 = false;
	switch (argc)
	{
	case(1):
		//Linux直接把case(1)这块去掉
		RegGetValueW(HKEY_CURRENT_USER, L"System\\GameConfigStore\\Children\\2c1ae850-e27e-4f10-a985-2dd951d15ba4", L"MatchedExeFullPath",
			RRF_RT_ANY, NULL, GameBinPath, &buffiersize);
		modspath = GameBinPath;
		modspath += L"\\..\\..\\mods\\";
		break;
	case(2):
		switch (argv[1][0])//其实是有空子可以钻的，比如说txsb进case t，cnm进case c,不过并没什么diao用就是了
		{
		case(L't'):
			modspath = argv[0];
			break;
		case(L'c'):
			std::wcout << s1 << endl
				<< s2 << endl;
			std::getline(wcin, modspath);
			清理 = true;
			break;
		default:
			break;
		}
		break;
	default:
		std::wcout << s3 << endl;
		std::getline(wcin, modspath);
		break;
	}

	path mods(modspath);
	//清理
	if (清理)
	{
		std::wcout << s4 << endl;
		for (auto dir : recursive_directory_iterator(mods))
		{
			if (dir.is_regular_file())
			{
				auto filepath = dir.path();
				if (regex_match(filepath.filename().string(), regex("(.*)(.tex)", regex::icase)))
				{
					const wstring file = canonical(filepath).wstring();
					std::wcout << file << endl;
					/////////////WINAPI/////////////
					DeleteFileW(file.c_str());
				}
			}
		}
		return 1;
	}

	PNGs.reserve(40);
	std::wcout << s5 << endl;
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
					std::wcout << s6 << endl;
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
	
	std::wcout << s7 << endl;

	unsigned long long clear_status = 0;
	unsigned long long converter_status = 0;
	auto cpuscount = thread::hardware_concurrency();
	for (unsigned char i = 0; i < cpuscount && i<64 ; i++)
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
	}
	std::wcout << s8 << endl;
	return 0;

	//在搞多线程
	
}

