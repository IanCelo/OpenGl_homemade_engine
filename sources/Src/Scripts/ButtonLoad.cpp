#include <Scripts/ButtonLoad.hpp>
#include <Core/Graph.hpp>
#include <Core/TimeManager.h>
#include <Core/InputsManager.hpp>

void ButtonLoad::action()
{
	//	Get singleton instances
	Core::TimeManager* _time = Core::TimeManager::instance();
	Core::InputsManager* _inputs = Core::InputsManager::instance();
	Core::Graph* _graph = Core::Graph::instance();

	//	Set time scale to 1 to unpause
	_time->timeScale = 1.f;

	//	Display mouse
	_inputs->displayMouse();

	//	DO NOT TELL the graph to unload Main Menu scene, we want to keep it

	//	Tell the graph which scene to switch in
	_graph->m_nextScene = "Assets/SavedScene.scn";
}