#include "pch.h"
#include "localizition.h"
#include <iostream>
#include <fstream>
using namespace std;


//Default stringtables
constexpr auto s1 = L"清理指定文件夹内的所有tex和XML文件\n\n\n\n";
constexpr auto s2 = L"输入文件夹的路径\n";//path
constexpr auto s3 = L"输入mods文件夹的路径以开始转换：\n";//modspath
constexpr auto s4 = L"\n==========================开始转换========================\n\n";//convert
constexpr auto s5 = L"========================开始遍历文件======================\n\n\n\n";//iterate files
constexpr auto s6 = L"文件/文件夹名乱码";//gamble
constexpr auto s7 = L"完成";//Done.
constexpr auto s8 = L"开始清理";
localization::localization()
{
	wchar_t lang[8] = { 0 };
	{
		char terrority[64] = { 0 };
		ifstream cfg("cfg");
		if (cfg.is_open())
		{
			char mbslang[16] = { 0 };
			cfg >> terrority;
			cfg >> mbslang;
			mbstowcs_s<8>(nullptr, lang, mbslang, 16);
			wcout.imbue(locale(terrority));
			wcerr.imbue(locale(terrority));
		}
		else
		{
			wcout.imbue(locale("chs"));
			wcerr.imbue(locale("chs"));
			table.emplace(1, s1);
			table.emplace(2, s2);
			table.emplace(3, s3);
			table.emplace(4, s4);
			table.emplace(5, s5);
			table.emplace(6, s6);
			table.emplace(7, s7);
			table.emplace(8, s8);
			return;
		}
	}

	FILE* stable = nullptr;
	auto err = _wfopen_s(&stable, lang, L"r");
	if (stable == nullptr)
	{
		cerr << "at ::localization::localization\n\terrno = " << err << endl;
		return;
	}

	while (!feof(stable))
	{
		wchar_t v[128] = { 0 };
		uint16_t k = 0;
		fscanf_s(stable, "%hd=%ls\n", &k, v, 128);
		table.emplace(k, v);
	}
}

std::wstring& localization::operator[](unsigned short i)
{
	static wstring r1 = L"\n";
	try
	{
		wstring& r = table[i];
		return r;
	}
	catch (const std::out_of_range&)
	{
		return  r1;
	}
	catch (const std::exception& e)
	{
		cerr << e.what() << endl;
	}

	return r1;
}


