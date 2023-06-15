#pragma once

#include <Utils/Singleton.h>

struct GLFWwindow;

namespace Core
{
	class Window : public Singleton<Window>
	{
	public:
		Window();

		GLFWwindow* m_window = nullptr;

		int		m_width;
		int		m_height;

		float	m_widthf;
		float	m_heightf;

		float	m_windowCoef;
		bool	m_wasReshaped = false;

		void update();
	};
}