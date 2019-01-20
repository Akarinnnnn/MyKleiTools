#include "CreateAtlas.h"


void KTEXlib::CreateAltas(std::wstring filename)
{
	using namespace pugi;
	xml_document AtlasXMLDoc;
	auto nodeAtlas = AtlasXMLDoc.append_child(L"Atlas");
	auto nodeTex = nodeAtlas.append_child(L"Texture");
	auto nodeElems = nodeAtlas.append_child(L"Elements");
	auto attFilename = nodeTex.append_attribute(filename.c_str());
	auto nodeElem = nodeElems.append_child(L"Element");
	auto attElemName = nodeElem.append_attribute(L"name");
	attElemName.as_string(filename.c_str());

}
