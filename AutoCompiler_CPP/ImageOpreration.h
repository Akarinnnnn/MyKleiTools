#pragma once

namespace ImageOperation
{ 
	bool ExportImage(std::wstring InputFileName);
	bool Convert(std::wstring sourcefile,std::wstring destfile);
}
namespace ImageOperation_2//With stdexception
{ 
	bool ExportImage(std::wstring InputFileName);
	bool Convert(std::wstring sourcefile,std::wstring destfile);
}