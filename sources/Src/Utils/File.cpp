#include <iostream>
#include <Utils/File.h>
#include <Utils/StringExtractor.h>

#include <Core/Log.hpp>

bool FileParser::openFile(const char* filename, std::ifstream& out)
{
	Core::Log* _log = Core::Log::instance();

	//	Open file in read mode
	//	----------------------
	out.open(filename);

	//	If failed return
	//	---------------
	if (!out)
	{
		_log->writeError("Failed to open file \"" + std::string(filename) + "\"");

		return false;
	}
	_log->writeSuccess("Opened file \"" + std::string(filename) + "\"");
	return true;
}



void FileParser::separatePathAndName(std::string& filePath, std::string& filename, std::istringstream& lineStream)
{
	std::string fullPath;
	lineStream >> fullPath;

	filePath = Extractor::ExtractDirectory(fullPath);
	filename = Extractor::ExtractFilename(fullPath);
}


