#pragma once
#include <Scripts/Button.hpp>

class ButtonPlayNewGame : public ButtonAction
{
public:
	ButtonPlayNewGame() { m_name = "ButtonPlayNewGame"; }
	void action() override;
};
