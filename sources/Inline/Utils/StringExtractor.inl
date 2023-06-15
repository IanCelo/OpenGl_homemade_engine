
inline std::string Extractor::ExtractNameWithoutExtension(const std::string& fileName)
{
	return Extractor::ExtractBeforeLast(fileName, '.', false);//fileName.substr(0, fileName.find_last_of('.'));;
}

inline std::string Extractor::ExtractDirectory(const std::string& fullPath)
{
	return ExtractBeforeLast(fullPath, '/', true);// fullPath.substr(0, fullPath.find_last_of('/') + 1);
}

inline std::string Extractor::ExtractFilename(const std::string& fullPath)
{
	return ExtractAfterLast(fullPath, '/', false); //fullPath.substr(fullPath.find_last_of('/') + 1);
}

inline std::string Extractor::ExtractBeforeLast(const std::string& fullPath, const char c, const bool include)
{
	if (include) return fullPath.substr(0, fullPath.find_last_of(c) + 1);
	return fullPath.substr(0, fullPath.find_last_of(c));
}

inline std::string Extractor::ExtractAfterLast(const std::string& fullPath, const char c, const bool include)
{
	if(include) return fullPath.substr(fullPath.find_last_of(c));
	return fullPath.substr(fullPath.find_last_of(c) +1);
}


inline int  Extractor::ExtractNumberAtEnd(std::string& name)
{
	size_t i = 0;
	for (; i < name.length(); i++) 
	{ 
		if (isdigit(name[i])) break;
	}
	std::string num = name.substr(i, name.length() - i);
	name = name.substr(0, i);
	return atoi(num.c_str());
}
