#pragma once
#include <Scripts/Button.hpp>

class ButtonLoad : public ButtonAction
{
public:
	ButtonLoad() { m_name = "ButtonLoad"; }
	void action() override;
};


