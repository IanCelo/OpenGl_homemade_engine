#pragma once

#include <string>

namespace Extractor
{
	//	Extract pure name of the file without extension
	//	Parameters : const std::string& fileName
	//	----------------------------------------
	std::string ExtractNameWithoutExtension(const std::string& fileName);

	//	Extract directory location
	//	Parameters : const std::string& fullPath
	//	----------------------------------------
	std::string ExtractDirectory(const std::string& fullPath);

	//	Extract filename
	//	Parameters : const std::string& fullPath
	//	----------------------------------------
	std::string ExtractFilename(const std::string& fullPath);

	//	Extract string before last char
	//	Parameters : const std::string& fullPath, const char c
	//	------------------------------------------------------
	std::string ExtractBeforeLast(const std::string& fullPath, const char c, const bool include);

	//	Extract string after last char
	//	Parameters : const std::string& fullPath, const char c
	//	------------------------------------------------------
	std::string ExtractAfterLast(const std::string& fullPath, const char c, const bool include);


	//	Extract string number and remove it from the referenced string 
	//	Parameters : const std::string& fullPath, const char c
	//	------------------------------------------------------
	int ExtractNumberAtEnd(std::string& name);
}

#include "../Inline/Utils/StringExtractor.inl"