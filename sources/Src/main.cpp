#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <time.h> 

#include <iostream>
#include <API.hpp>


int main()
{
	{
		srand((unsigned int)time(NULL));

		API m_api;
		if (m_api.init() < 0) return -1;

		m_api.loading();

		m_api.windowLoop();
	}

	_CrtDumpMemoryLeaks();
	return 0;
}