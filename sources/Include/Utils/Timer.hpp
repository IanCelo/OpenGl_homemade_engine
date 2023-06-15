#pragma once

#include <Core/TimeManager.h>
#include <iostream>

// Timer starts at 0.f, and then ended return true when the time elapsed is higher than the endTime;
class Timer 
{
public:
	Timer(float time = 1.f, bool startActive = false) : m_endTime(time), m_active(startActive) {}
	
	bool isActive()    { return m_active;              } 
	bool ended()	   { return m_crtTime > m_endTime; }

	float getTime()	   { return m_crtTime;     }
	void on()		   { m_active = true;	   }
	void off()		   { m_active = false;	   }
	void switchState() { m_active = !m_active; }
	
	// Set elapsed time to 0.f + off the timer
	void reset()	   { restart(); off(); }
	// Set elapsed time to 0.f 
	void restart()	   { m_crtTime = 0.f;  }
	// Apply deltaTime to elapsed time
	void update()      { m_crtTime += Core::TimeManager::instance()->deltaTime; }
	
	//Set the timer duration
	void setEndTime(float time) { m_endTime = time; }
	//Display Timer Infos in console
	void showInConsole(int i) 
	{
		std::cout << "Timer[" << i << "]: "<< std::endl;
		std::cout << "	- crtTime: "<<m_crtTime<< std::endl;
		std::cout << "  - endTime: "<<m_endTime << std::endl;
		std::cout << "  - Active: " << m_active<<" / "<< "  - Ended: " << ended() << std::endl;
	}

	float m_endTime = 0.f;
private:

	float m_crtTime = 0.f;

	bool m_active = false;
};