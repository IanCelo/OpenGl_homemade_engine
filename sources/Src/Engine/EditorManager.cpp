#include <Engine/EditorManager.hpp>


#include <Core/Graph.hpp>

namespace Core
{
	void EditorManager::init(GLFWwindow* in_window)
	{
		//  Setup Dear ImGui

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable Docking

		ImGui::StyleColorsDark();
		ImGuiStyle& style = ImGui::GetStyle();

		ImGui_ImplGlfw_InitForOpenGL(in_window, true);
		ImGui_ImplOpenGL3_Init("#version 450");

		m_themeColor = ImVec4(0.150f, 0.125f, 0.115f, .9f);
		m_lightThemeColor = ImVec4(m_themeColor.x + .060f, m_themeColor.y + .060f, m_themeColor.z + .060f, .950f);
		m_darkThemeColor = ImVec4(m_themeColor.x - .070f, m_themeColor.y - .070f, m_themeColor.z - .070f, .950f);

		setTheme();
	}


	void EditorManager::setTheme()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

		ImGui::PushStyleColor(ImGuiCol_WindowBg, m_themeColor);
		ImGui::PushStyleColor(ImGuiCol_ChildBg, m_themeColor);
		ImGui::PushStyleColor(ImGuiCol_PopupBg, m_themeColor);

		ImGui::PushStyleColor(ImGuiCol_Border, m_darkThemeColor);
		ImGui::PushStyleColor(ImGuiCol_TitleBg, m_darkThemeColor);
		ImGui::PushStyleColor(ImGuiCol_TitleBgActive, m_darkThemeColor);

		ImGui::PushStyleColor(ImGuiCol_FrameBg, m_lightThemeColor);
		ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, m_themeColor);
		ImGui::PushStyleColor(ImGuiCol_FrameBgActive, m_darkThemeColor);

		ImGui::PushStyleColor(ImGuiCol_Header, m_darkThemeColor);
		ImGui::PushStyleColor(ImGuiCol_HeaderHovered, m_darkThemeColor);
		ImGui::PushStyleColor(ImGuiCol_HeaderActive, m_darkThemeColor);

		ImGui::PushStyleColor(ImGuiCol_Button, m_lightThemeColor);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, m_darkThemeColor);
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, m_darkThemeColor);

		ImGui::PushStyleColor(ImGuiCol_ResizeGrip, m_darkThemeColor);
		ImGui::PushStyleColor(ImGuiCol_ResizeGripHovered, m_darkThemeColor);
		ImGui::PushStyleColor(ImGuiCol_ResizeGripActive, m_darkThemeColor);

		ImGui::PushStyleColor(ImGuiCol_Separator, m_darkThemeColor);
		ImGui::PushStyleColor(ImGuiCol_SeparatorHovered, m_darkThemeColor);
		ImGui::PushStyleColor(ImGuiCol_SeparatorActive, m_darkThemeColor);

		ImGui::PushStyleColor(ImGuiCol_SliderGrab, m_darkThemeColor);
		ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, m_darkThemeColor);
		ImGui::PushStyleColor(ImGuiCol_CheckMark, m_darkThemeColor);
	}


	void EditorManager::resetTheme()
	{
		popTheme();

		setTheme();
	}


	void EditorManager::popTheme()
	{
		ImGui::PopStyleColor(24);
		ImGui::PopStyleVar(2);
	}


	void EditorManager::updateEditorWindows()
	{
		if (ImGui::Begin("Parameters"))
		{
			if (ImGui::SliderFloat4("Theme", &m_themeColor.x, 0.f, 1.f))
			{
				resetTheme();
			}
		}
		ImGui::End();

		//	Display debug controls
		if (ImGui::Begin("Config"))
		{
			m_graph->showImGUIGraph();
		}
		ImGui::End();

		if (ImGui::Begin("Scene"))
		{
			m_graph->showImGUIScene();
		}
		ImGui::End();

		if (ImGui::Begin("Inspector"))
		{
			m_graph->showImGUIGameObject();
		}
		ImGui::End();

		m_graph->showImGUIResourcesManager();
	}
}