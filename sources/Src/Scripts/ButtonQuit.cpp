#include <Scripts/ButtonQuit.hpp>
#include <Core/Graph.hpp>

void ButtonQuit::action()
{
	//	Get singleton instances
	Core::Graph* _graph = Core::Graph::instance();

	//	Tell the graph we want to quit the game
	_graph->m_quit = true;
}