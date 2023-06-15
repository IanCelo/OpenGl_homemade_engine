#pragma once

#include <string>
#include <fstream>
#include <sstream>

#include <Maths/Vector4.h>

namespace FileParser
{
	//	Open File and retrun true if successfully opened
	//	Parameters : const char* filename, ifstream& out
	//	------------------------------------------------
	bool openFile(const char* filename, std::ifstream& out);

	//	Open File and retrun true if successfully opened
	//	Parameters : const string& filename, ifstream& out
	//	--------------------------------------------------
	bool openFile(const std::string& filename, std::ifstream& out);

	//	Open File and retrun true if successfully opened
	//	Parameters : const char* path, const char* filename, ifstream& out
	//	------------------------------------------------------------------
	bool openFile(const char* path, const char* filename, std::ifstream& out);

	//	Separate file path and file name in two string
	//	Parameters : std::string& filePath, std::string& filename, std::istringstream& lineStream
	//	-----------------------------------------------------------------------------------------
	void separatePathAndName(std::string& filePath, std::string& filename, std::istringstream& lineStream);


	//	Check if the file has the right extension
	//	Parameters : onst std::string& fileName, const std::string& expectedExtension
	//	----------------------------------------------------------------
	bool CheckExtension(const std::string& fileName, const std::string& expectedExtension);


	//	Check if the file has one of the extension in the list
	//	Parameters : onst std::string& fileName, const std::string* expectedExtension, size_t size
	//	------------------------------------------------------------------------------------------
	bool CheckExtension(const std::string& fileName, const std::string* acceptedExtensionList, size_t size);


	//	Get functions
	//	-------------

	float getFloat(std::istringstream& line);
	int getInt(std::istringstream& line);
	bool getBool(std::istringstream& line);

	Maths::Vector4f getVector4(std::istringstream& line);
	Maths::Vector3f getVector3(std::istringstream& line);
	Maths::Vector2f getVector2(std::istringstream& line);

	std::string getString(std::istringstream& line);
}

namespace FileWriter
{
	//	Write a Vector2 in file
	//	Parameters : std::ofstream& file, Maths::Vector2f a
	//	---------------------------------------------------
	void writeVec2InFile(std::ofstream& file, Maths::Vector2f a);

	//	Write a Vector3 in file
	//	Parameters : std::ofstream& file, Maths::Vector3f a
	//	---------------------------------------------------
	void writeVec3InFile(std::ofstream& file, Maths::Vector3f a);

	//	Write a Vector4 in file
	//	Parameters : std::ofstream& file, Maths::Vector4f a
	//	---------------------------------------------------
	void writeVec4InFile(std::ofstream& file, Maths::Vector4f a);
}


#include "../Inline/Utils/File.inl"