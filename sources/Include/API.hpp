#pragma once

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <Resources/Scene.hpp>
#include <Engine/EditorManager.hpp>


struct GLFWwindow;

class API
{
public:

	//	Constructor
	//	-----------

	API() = default;

	//	Internal Public Function
	//	------------------------

	int		init();
	void	loading();
	void	windowLoop();

private:

	//	Internal Private Function
	//	-------------------------

	GLFWwindow* m_window;
	Core::EditorManager m_editor;
};