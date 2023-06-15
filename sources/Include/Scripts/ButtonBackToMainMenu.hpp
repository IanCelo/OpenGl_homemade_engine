#pragma once
#include <Scripts/Button.hpp>

class ButtonBackToMainMenu : public ButtonAction
{
public:
	ButtonBackToMainMenu() { m_name = "ButtonBackToMainMenu"; }
	void action() override;
};


