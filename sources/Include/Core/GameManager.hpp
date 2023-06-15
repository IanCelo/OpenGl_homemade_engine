#pragma once

#include <Utils/Timer.hpp>
#include <Utils/Singleton.h>

#include <Engine/GameObject.hpp>
#include <unordered_map>

namespace Core
{
	class GameManager : public Singleton<GameManager>
	{
	private:
		std::unordered_map<int, GameObject*> m_enemyList;
		Timer m_timeBeforeReturn = Timer(2.f, false);
		bool m_isOn = false;

	public:
		
		void update();
		int addEnemyToList(GameObject* enemy);
		void removeEnemyFromList(int index);
		void WinOrLoseGame(bool win);
	};
}