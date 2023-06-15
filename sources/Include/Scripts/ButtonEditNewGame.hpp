#pragma once
#include <Scripts/Button.hpp>

class ButtonEditNewGame : public ButtonAction
{
public:
	ButtonEditNewGame() { m_name = "ButtonPlayNewGame"; }
	void action() override;
};
