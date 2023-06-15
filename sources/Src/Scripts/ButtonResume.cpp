#include <Scripts/ButtonResume.hpp>
#include <Core/Graph.hpp>
#include <Resources/Scene.hpp>

void ButtonResume::action()
{
	//	Get singleton instances
	Core::Graph* _graph = Core::Graph::instance();

	//	UnPause the game
	_graph->getCurrentScene()->m_pause = false;

	//	Call pause function of current scene so it can resume
	_graph->getCurrentScene()->pause();
}