#pragma once

#include <Utils/Singleton.h>

class Timer;

namespace Core
{
	class TimeManager : public Singleton<TimeManager>
	{
	private:
		double oldTime = 0.0f;
		double newTime = 0.0f;

		float timeStock = 0.f;
		float deltaTimeUnscaled = 0.f;

	public:
		float deltaTime = 0.f;
		float fixedDeltaTime = 0.02f;
		float timeScale = 1.f;

		void setDeltaTime();
		void setFixedDeltaTime();

		bool fixedDeltaTimeLoop();

		float getTime();
	};
}