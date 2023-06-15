#pragma once

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

struct GLFWwindow;

namespace Core
{
	class Graph;

	class EditorManager
	{
	private:

		//	Private Variables
		//	-----------------

		ImVec4 m_lightThemeColor;
		ImVec4 m_darkThemeColor;

	public:

		//	Public Variables
		//	----------------

		Graph* m_graph;
		ImVec4 m_themeColor;

		//	Functions
		//	---------

		void init(GLFWwindow* in_window);
		void setTheme();
		void resetTheme();
		void popTheme();
		void updateEditorWindows();
	};
}