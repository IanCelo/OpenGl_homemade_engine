#pragma once
#include <Scripts/Button.hpp>

class ButtonResume : public ButtonAction
{
public:
	ButtonResume() { m_name = "ButtonResume"; }
	void action() override;
};


