#pragma once

#include <string>

#include "Utils/Singleton.h"

namespace Core
{
	class Log : public Singleton<Log>
	{
	private:
		//	Private Internal Variables
		//	-------------------------

		//	Log file name
		std::string m_logName;

		//	Content written in log during its creation
		std::string m_content;

		//	Write the string in the content
		//	Parameters : string in_line
		//	---------------------------
		void writeInContent(const std::string& in_line);

	public:
		//	Constructor & Destructor
		//	------------------------

		Log();

		~Log();


		//	Public Internal Functions
		//	-------------------------

		//	Write the string in the content
		//	Parameters : string in_line
		//	---------------------------
		void write(const std::string& in_line);

		//	Write a Success line in the content
		//	Parameters : string in_line
		//	---------------------------
		void writeSuccess(const std::string& in_line);

		//	Write a Failure line in the content
		//	Parameters : string in_line
		//	---------------------------
		void writeFailure(const std::string& in_line);

		//	Write a Waring line in the content
		//	Parameters : string in_line
		//	---------------------------
		void writeWarning(const std::string& in_line);

		//	Write an Error line in the content
		//	Parameters : string in_line
		//	---------------------------
		void writeError(const std::string& in_line);

		//	Break a line
		//	Parameters : none
		//	-----------------
		void breakLine();
	};
}