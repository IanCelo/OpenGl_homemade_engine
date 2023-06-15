#include <Scripts/ButtonPlayNewGame.hpp>
#include <Core/Graph.hpp>
#include <Core/TimeManager.h>
#include <Core/InputsManager.hpp>
#include <Scripts/Button.hpp>

void ButtonPlayNewGame::action()
{
	//	Get singleton instances
	Core::TimeManager* _time = Core::TimeManager::instance();
	Core::InputsManager* _inputs = Core::InputsManager::instance();
	Core::Graph* _graph = Core::Graph::instance();

	//	Reset time scale to 1 to unpause
	_time->timeScale = 1.f;

	//	Hide mouse
	_inputs->hideMouse();

	//	Set mode to Full play mode
	_graph->m_mode = EngineMode::FULLPLAYMODE;

	//	DO NOT TELL the graph to unload Main Menu scene, we want to keep it

	//	Tell the graph which scene to switch in
	_graph->m_nextScene = "Assets/NewGame.scn";
}