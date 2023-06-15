#include <Scripts/ButtonBackToMainMenu.hpp>
#include <Core/Graph.hpp>
#include <Core/InputsManager.hpp>
#include <Resources/Scene.hpp>

void ButtonBackToMainMenu::action()
{
	//	Get singleton instances
	Core::Graph* _graph = Core::Graph::instance();
	Core::InputsManager* _inputs = Core::InputsManager::instance();

	//	Display mouse, to be sure it is enabled in main menu
	_inputs->displayMouse();

	//	Tell the graph it can unload the current scene
	_graph->m_unload = true;

	//	Set mode to Full play mode
	_graph->m_mode = EngineMode::FULLPLAYMODE;

	//	Tell the graph which scene to switch in
	_graph->m_nextScene = "Assets/MainMenu.scn";
}