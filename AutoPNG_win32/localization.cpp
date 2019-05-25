#include "localizition.h"

using namespace std;

#include <cstdio>
//Default stringtables
auto s1 = L"清理指定文件夹内的所有tex文件\n\n\n\n";
auto s2 = L"输入文件夹的路径\n";//path
auto s3 = L"输入mods文件夹的路径以开始转换：\n";//modspath
auto s4 = L"==========================开始转换========================\n\n";//convert
auto s5 = L"========================开始遍历文件======================\n\n\n\n";//iterate files
auto s6 = L"文件/文件夹名乱码";//gamble
auto s8 = L"完成";//Done.
auto s9 = L"开始清理";
localizition::localizition()
{
	FILE* stable = nullptr;
	auto err = _wfopen_s(&stable, L"stringtables.txt", L"r");
	while (!feof(stable))
	{
		fscanf_s(stable, "%s\n")
	}
}