#include "AltasGen.h"
using namespace ktexlib::AtlasGen;
using namespace std;

void ktexlib::AtlasGen::Atlas(imgvec imgs, string output, unsigned int max_altas_size, unsigned short scale_factor)
{

}


void ktexlib::AtlasGen::AtlasDocumentGen(wstring filename, bbox bbox, ldb offset = 0.5)
{
	using namespace pugi;

	ofstream fsXML(filename);
	if (fsXML.is_open())
	{
		cout << "´ò¿ªÊ§°Ü" << endl;
	}

	xml_document AtlasXMLDocument;
	auto Atlas = AtlasXMLDocument.append_child(L"Atlas");

		auto Texture = Atlas.append_child(L"Texture");
			auto tex_filename = Texture.append_attribute(L"filename");
				tex_filename.as_string(filename.c_str());

		auto Elements = Atlas.append_child(L"Elements");
			auto Element = Elements.append_child(L"Element");
				auto e_u1 = Element.append_attribute(L"u1");
				auto e_u2 = Element.append_attribute(L"u2");
				auto e_v1 = Element.append_attribute(L"v1");
				auto e_v2 = Element.append_attribute(L"v2");

	//offset
	double offsets[2]{ offset };//0¡úx, 1¡úy
	double UVs[4]{ 0 };//0~4 u1,u2,v1,v2
	
	/*
	python lambda
	lambda arg:func

	C++ lambda
	[](arg){func}
	*/

	fsXML << AtlasXMLDocument;
	fsXML.close();
}
