#include <iostream>
#include "../ktexlib/AltasGen.h"

int main()
{
	using namespace ktexlib::AtlasGen;
	bbox testbbox =
	{
		0,0,2,2
	};
	unsigned short size[2] = { 2,2 };
	AtlasDocumentGen(L"SB",size , testbbox);
}
