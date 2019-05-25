// AutoPNG_win32.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include "localizition.h"

//STL
#include <iostream>
#include <regex>
#include <filesystem>//MSVC特色
#include <thread>
#include <atomic>
#include <exception>
#include <system_error>
#include <mutex>
//PNG处理
#include <lodepng.h>
//Win32
#ifdef _WIN32
#include "windows.h"
#endif
#define MULTI_THREAD_KTEXCONOUTPUT
#include "TEXFileOperation.h"

//可能的注册表项: HKEY_CURRENT_USER\System\GameConfigStore\Children\2c1ae850-e27e-4f10-a985-2dd951d15ba4
//
using namespace std;

namespace MAIN
{
	std::mutex strmutex;
	std::mutex imutex;
	std::mutex con;
	std::vector<string> PNGs;
}
void convert_func(vector<string>& str,unsigned int& i)
{
	string pngfile;
	lodepng::State pngstate;
	ktexlib::uc_vector pngfvec;
	ktexlib::KTEXFileOperation::KTEX ktexop;
	ktexlib::KTEXFileOperation::RGBAv2 rgba;
	while (true)
	{
		MAIN::strmutex.lock();
		if (str.empty())
		{
			MAIN::strmutex.unlock();
			str.shrink_to_fit();
			break;
		}
		pngfile = str.back();
		str.pop_back();
		MAIN::strmutex.unlock();
		ktexop.clear();
		lodepng::load_file(pngfvec, pngfile);
		unsigned int w, h = 0;
		auto ret = lodepng::decode(rgba.data, w, h, pngstate, pngfvec);
		//判断
		if (ret!=0)
		{
			cerr << lodepng_error_text(ret) << endl;
			continue;
		}
		if (w > USHRT_MAX || h > USHRT_MAX)
			continue;
		//赋值
		rgba.width = w;
		rgba.height = h;
		ktexop.output = filesystem::path(pngfile).replace_extension(".tex");
		MAIN::con.lock();
		wcout << ktexop.output << endl;
		MAIN::con.unlock();
		//转换
		ktexop.PushRGBA(rgba,1);
		
		ktexop.Convert();
		//清理
		ktexop.clear();
		rgba.data.clear();
		pngfvec.clear();
	}
	MAIN::imutex.lock();
	i++;
	MAIN::imutex.unlock();
}

int wmain(int argc,wchar_t* argv[])
 {
	localization local;
	////////////////////也是MSVC特色/////////////////////////
	using namespace std::filesystem;
	unsigned long buffiersize = MAX_PATH;
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
			std::wcout << local[1] << endl
				<< local[2] << endl;
			std::getline(wcin, modspath);
			清理 = true;
			break;
		default:
			break;
		}
		break;
	default:
		std::wcout << local[3] << endl;
		std::getline(wcin, modspath);
		break;
	}

	path mods(modspath);
	//清理
	if (清理)
	{
		std::wcout << local[8] << endl;
		for (auto dir : recursive_directory_iterator(mods))
		{
			if (dir.is_regular_file())
			{
				auto filepath = dir.path();
				auto fileext = filepath.extension().wstring();
				if (regex_match(fileext, wregex(L".tex", wregex::icase)) || regex_match(fileext,wregex(L".xml",wregex::icase)))
				{
					const wstring file = canonical(filepath).wstring();
					std::wcout << file << endl;
					filesystem::remove(filepath);
				}
			}
		}
		return 1;
	}

	MAIN::PNGs.reserve(40);
	std::wcout << local[5] << endl;
	for (auto dir : directory_iterator(mods))
	{
		regex png(".png", regex_constants::icase);
		if (dir.is_directory())
		{
			auto images = canonical(dir.path()) / "images";
			auto bp = canonical(dir.path()) / "bigportraits";
			try
			{
				for (auto entries : recursive_directory_iterator(images))
				{
					try
					{
						if (
							entries.is_regular_file() &&
							regex_match(entries.path().extension().string(),png)
							)
						{
							wcout << entries.path().wstring() << endl;
							MAIN::PNGs.push_back(entries.path().string());
						}
					}
					catch (std::filesystem::filesystem_error& e)
					{
						int errcode = e.code().value();
						if (errcode == 3);//就是单纯的找不到文件 
						else
							cerr << e.what() << endl;
					}
					catch (system_error& e)
					{
						cerr << e.what() << endl;
						if (e.code().value() == 1113)
							std::wcout << local[6] << endl;
					}
					catch (exception& e)
					{
						cerr << e.what() << endl;
					}
					catch (...)
					{
						wcerr << L"未知错误" << endl;
					}
				}
			}
			catch (std::filesystem::filesystem_error e)
			{
				if (e.code().value() == 3);
				else
					cerr << e.what() << endl;
			}
			try
			{
				for (auto entries : recursive_directory_iterator(bp))
				{
					try 
					{
						if (entries.is_regular_file() &&
						regex_match(entries.path().extension().string(), png)
						)
						{
						wcout << entries.path().wstring() << endl;
						MAIN::PNGs.push_back(entries.path().string());
						}
					}
					catch (std::filesystem::filesystem_error& e)
					{
						int errcode = e.code().value();
						if (errcode == 3)//就是单纯的找不到文件 
						{
						}
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
							std::wcout << local[6] << endl;
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
			catch (std::filesystem::filesystem_error e)
			{
				if (e.code().value() == 3);
				else
					cerr << e.what() << endl;
			}
		}

	}
	
	std::wcout << local[4] << endl;
	unsigned int converter_status = 0;
	auto cpuscount = thread::hardware_concurrency();
	for (unsigned int i = 0; i < cpuscount; i++)
	{
		thread converter(convert_func,ref(MAIN::PNGs), ref(converter_status));
		converter.detach();
	}
	while (converter_status!=cpuscount)
	{
		Sleep(500);
	}
	std::wcout << local[7] << endl;
	return 0;
}
