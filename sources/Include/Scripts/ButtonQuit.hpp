#pragma once
#include <Scripts/Button.hpp>

class ButtonQuit : public ButtonAction
{
public:
	ButtonQuit() { m_name = "ButtonQuit"; }
	void action() override;
};


