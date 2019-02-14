#include "AltasGen.h"
using namespace ktexlib::AtlasGen;
using namespace std;

void ktexlib::AtlasGen::Atlas(imgvec imgs, string output, unsigned int max_altas_size, unsigned short scale_factor)
{

}


void ktexlib::AtlasGen::AtlasDocumentGen(wstring filename, unsigned short imgsize[2], bbox bbox, double offset)
{
	using namespace pugi;

	/*ofstream fsXML(filename);
	if (!fsXML.is_open())
	{
		cout << "´ò¿ªÊ§°Ü" << endl;
	}*/

	xml_document AtlasXMLDocument;
	auto Atlas = AtlasXMLDocument.append_child(L"Atlas");

		auto Texture = Atlas.append_child(L"Texture");
			auto tex_filename = Texture.append_attribute(L"filename");
				tex_filename.set_value((filename+L".tex").c_str());

		auto Elements = Atlas.append_child(L"Elements");
			auto Element = Elements.append_child(L"Element");
				auto e_name = Element.append_attribute(L"name");
					e_name.set_value((filename + L".tex").c_str());
				auto e_u1 = Element.append_attribute(L"u1");
				auto e_u2 = Element.append_attribute(L"u2");
				auto e_v1 = Element.append_attribute(L"v1");
				auto e_v2 = Element.append_attribute(L"v2");


	//offset
	double border_uv_offsets[2]{ offset };//0¡úx, 1¡úy
	//0~4 u1,u2,v1,v2
	double UVs[4] =
	{
		 clamp(bbox.x / imgsize[0] + border_uv_offsets[0],0.0,1.0),			  //u1
		 clamp((bbox.y + bbox.h) / imgsize[1] + border_uv_offsets[1],0.0,1.0),//u2
		 clamp((bbox.x + bbox.w) / imgsize[0] - border_uv_offsets[0],0.0,1.0),//v1
		 clamp(bbox.y / imgsize[0] - border_uv_offsets[1],0.0,1.0)			  //v2
	};
	
	

	e_u1.set_value(UVs[0]);
	e_u2.set_value(UVs[1]);
	e_v1.set_value(UVs[2]);
	e_v2.set_value(UVs[3]);

	/*
	python lambda
	lambda arg:func

	C++ lambda
	[arg]{func}
	*/

	AtlasXMLDocument.save_file((filename + L".xml").c_str(), L"\t", 1, pugi::encoding_utf8);
}
