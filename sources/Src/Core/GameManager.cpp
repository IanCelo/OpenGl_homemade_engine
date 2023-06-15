#include <Core/GameManager.hpp>
#include <Core/Graph.hpp>
#include <Core/InputsManager.hpp>
#include <LowRenderer/Text.hpp>

using namespace Core;

void GameManager::update()
{
	if (!m_isOn)
		return;

	if (m_enemyList.empty())
		WinOrLoseGame(true);

	if (m_timeBeforeReturn.isActive())
		m_timeBeforeReturn.update();
}

int GameManager::addEnemyToList(GameObject* enemy)
{
	if (!m_isOn)
		m_isOn = true;

	int key = (int)m_enemyList.size();
	m_enemyList[key] = enemy;
	return key;
}

void GameManager::removeEnemyFromList(int index)
{
	m_enemyList.erase(index);
}

void GameManager::WinOrLoseGame(bool win)
{
	if (win)
	{
		TextRender::instance()->AddText("abnes.ttf", "YOU WON ", { -4, 3 }, 1.f, { 0.f, 1.f, 0.f });
	}
	else
	{
		TextRender::instance()->AddText("abnes.ttf", "YOU LOSE", { -4, 3 }, 1.f, { 1.f, 0.f, 0.f });
	}

	if (!m_timeBeforeReturn.isActive())
		m_timeBeforeReturn.on();
	else if (m_timeBeforeReturn.ended())
	{
		Core::Graph* _graph = Core::Graph::instance();
		Core::InputsManager* _inputs = Core::InputsManager::instance();

		_inputs->displayMouse();
		_graph->m_unload = true;
		_graph->m_mode = EngineMode::FULLPLAYMODE;
		_graph->m_nextScene = "Assets/MainMenu.scn";

		m_isOn = false;
		m_timeBeforeReturn.reset();
	}
}