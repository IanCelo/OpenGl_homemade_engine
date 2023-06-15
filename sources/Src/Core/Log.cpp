#include <iostream>
#include <ctime>


#include <fstream>
#include <sstream>

#include <Core/Log.hpp>

#define MAX_S 64


void getTime(char* c)
{
	//	Set Time
	time_t now = time(0);
	struct tm time;

	#if defined(__unix__)
		localtime_r(&time, &now);
	#elif defined(_MSC_VER)
		localtime_s(&time, &now);
	#endif

	//	Create char array c with a specific time format -> HH:MM:SS
	strftime(c, MAX_S, "%H:%M:%S", &time);
}

Core::Log::Log()
{
	//	Get time
	char c[MAX_S];

	//	Set Time
	time_t now = time(0);
	struct tm time;

	#if defined(__unix__)
		localtime_r(&time, &now);
	#elif defined(_MSC_VER)
		localtime_s(&time, &now);
	#endif

	//	Create char array c with a specific time format -> HH_MM_SS
	strftime(c, MAX_S, "%H_%M_%S", &time);

	//	Set log name
	m_logName = "Resource/Log/Log_" + std::string(c) + ".log";
}

Core::Log::~Log()
{
	//	Write its content in a file at he end
	//	-------------------------------------

	std::ofstream file(m_logName);

	if (file.is_open() == false)
	{
		writeError("Unable to create log file - The log won't be saved");
		return;
	}

	write("Closing program...");

	file << m_content;

	file.close();
}


void Core::Log::writeInContent(const std::string& in_line)
{
	//	Get time
	char c[MAX_S];
	getTime(c);

	//	Create new line
	std::string new_line = "[" + std::string(c) + "]" + "\t" + in_line + "\n";

	//	Display in console
	std::cout << new_line;

	//	Add in content
	m_content += new_line;
}

void Core::Log::write(const std::string& in_line)
{
	writeInContent("INFO    | " + in_line);
}

void Core::Log::writeSuccess(const std::string& in_line)
{
	writeInContent("SUCCESS | " + in_line);
}

void Core::Log::writeFailure(const std::string& in_line)
{
	writeInContent("FAILURE | " + in_line);
}

void Core::Log::writeWarning(const std::string& in_line)
{
	writeInContent("WARNING | " + in_line);
}

void Core::Log::writeError(const std::string& in_line)
{
	writeInContent("ERROR   | " + in_line);
}

void Core::Log::breakLine()
{
	writeInContent("        |");
	writeInContent("        |------------------------------------------------------------------------");
	writeInContent("        |");
}