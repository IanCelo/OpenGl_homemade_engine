#include <Core/TimeManager.h>

using namespace Core;

#include <GLFW/glfw3.h>
#include <Utils/Timer.hpp>

void TimeManager::setDeltaTime()
{
	newTime = glfwGetTime();
	deltaTimeUnscaled = (float)(newTime - oldTime);
	oldTime = newTime;

	deltaTime = deltaTimeUnscaled * timeScale;
}

void TimeManager::setFixedDeltaTime()
{
	timeStock += deltaTime;
}

bool TimeManager::fixedDeltaTimeLoop()
{
	if (timeStock > fixedDeltaTime)
	{
		timeStock -= fixedDeltaTime;
		return true;
	}

	return false;
}

float TimeManager::getTime()
{
	return (float)newTime;
}