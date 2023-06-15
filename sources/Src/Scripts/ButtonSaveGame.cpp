#include <Scripts/ButtonSaveGame.hpp>
#include <Core/Graph.hpp>
#include <Resources/Scene.hpp>

void ButtonSaveGame::action()
{
	//	Get singleton instances
	Core::Graph* _graph = Core::Graph::instance();

	//	Save current scene in "Assets/SavedScene.scn"
	_graph->getCurrentScene()->saveScene("Assets/SavedScene.scn");
}