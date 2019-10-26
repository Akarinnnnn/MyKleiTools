// CrashTexFinder.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <vector>
#include <filesystem>
#include "../library_collections/ktexlib_dymanic/TEXFileOperation.h"

using namespace std::filesystem;
using std::cout;
using std::cin;
using std::vector;

std::string to_string(ktexlib::KTEXFileOperation::platfrm val)
{
	using namespace std::string_literals;
	switch (val)
	{
	case ktexlib::KTEXFileOperation::platfrm::opengl:
		return "OpenGL"s;
	case ktexlib::KTEXFileOperation::platfrm::xb360:
		return "Xbox 360"s;
	case ktexlib::KTEXFileOperation::platfrm::ps3:
		return "PlayStation 3"s;
	case ktexlib::KTEXFileOperation::platfrm::unk:
		return "unknown"s;
	default:
		return "unknown value,uint " + std::to_string((unsigned int)val);
	}
}

int main()
{
	vector<path> crash_texs;
	for (auto& mod : directory_iterator())
	{
		for (auto& dir_info : directory_iterator(mod))
		{
			if (dir_info.is_regular_file())
			{
				path file = dir_info.path();
				if (file.extension() == ".tex")
				{
					using namespace ktexlib::KTEXFileOperation;
					KTEX tex;
					tex.LoadKTEX(file);
					if (tex.Info.platform != platfrm::opengl)
					{
						crash_texs.push_back(file);
						cout << file << "'s platform is " << to_string(tex.Info.platform) << '\n';
					}
				}
			}
		}
	}
	cout.flush();

	char answer = 0;

	cout << "delete them?[Y/N]" << std::endl;
	cin >> answer;

	if (answer == 'y' || answer == 'Y')
	{
		for (auto& tex : crash_texs)
		{
			remove(tex);
		}
	}
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
