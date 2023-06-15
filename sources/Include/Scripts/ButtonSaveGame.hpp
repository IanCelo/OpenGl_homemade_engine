#pragma once
#include <Scripts/Button.hpp>

class ButtonSaveGame : public ButtonAction
{
public:
	ButtonSaveGame() { m_name = "ButtonSaveGame"; }
	void action() override;
};


