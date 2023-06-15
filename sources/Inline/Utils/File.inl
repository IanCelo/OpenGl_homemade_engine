
inline bool FileParser::openFile(const std::string& filename, std::ifstream& out)
{
	const char* path = filename.c_str();
	return openFile(path, out);
}

inline bool FileParser::openFile(const char* path, const char* filename, std::ifstream& out)
{
	std::string filePath = std::string(path) + std::string(filename);
	return openFile(filePath, out);
}


inline float FileParser::getFloat(std::istringstream& line)
{
	float out;
	line >> out;
	return out;
}

inline int FileParser::getInt(std::istringstream& line)
{
	int out;
	line >> out;
	return out;
}

inline bool FileParser::getBool(std::istringstream& line)
{
	bool out;
	line >> out;
	return out;
}

inline Maths::Vector4f FileParser::getVector4(std::istringstream& line)
{
	Maths::Vector4f out;
	line >> out.x;
	line >> out.y;
	line >> out.z;
	line >> out.w;
	return out;
}

inline Maths::Vector3f FileParser::getVector3(std::istringstream& line)
{
	Maths::Vector3f out;
	line >> out.x;
	line >> out.y;
	line >> out.z;
	return out;
}


inline Maths::Vector2f FileParser::getVector2(std::istringstream& line)
{
	Maths::Vector2f out;
	line >> out.x;
	line >> out.y;
	return out;
}

inline std::string FileParser::getString(std::istringstream& line)
{
	std::string out;
	line >> out;
	return out;
}

inline bool FileParser::CheckExtension(const std::string& fileName, const std::string& expectedExtension)
{
	std::string ext = fileName.substr(fileName.find_last_of('.')+1);

	if (_strcmpi(ext.c_str(), expectedExtension.c_str()) == 0) return true;
	return false;
}


inline bool FileParser::CheckExtension(const std::string& fileName, const std::string* acceptedExtensionList, size_t size)
{
	std::string ext = fileName.substr(fileName.find_last_of('.') + 1);

	for(size_t i = 0 ; i < size; i++)
	{
		if (_strcmpi(ext.c_str(), acceptedExtensionList[i].c_str()) == 0) return true;
	}
	return false;
}


inline void FileWriter::writeVec2InFile(std::ofstream& file, Maths::Vector2f a)
{
	file << a.x << " ";
	file << a.y;
}

inline void FileWriter::writeVec3InFile(std::ofstream& file, Maths::Vector3f a)
{
	file << a.x << " ";
	file << a.y << " ";
	file << a.z;
}

inline void FileWriter::writeVec4InFile(std::ofstream& file, Maths::Vector4f a)
{
	file << a.x << " ";
	file << a.y << " ";
	file << a.z << " ";
	file << a.w;
}