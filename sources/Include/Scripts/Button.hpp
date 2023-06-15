#pragma once
#include <Engine/Component.hpp>

class BoxCollider2D;
class Sprite;

class Button;

class ButtonAction
{
public:
	Button* m_button = nullptr;
	std::string m_name;
	virtual void action() = 0;
};

class Button : public Component
{
public:
	//	Constructor
	//	-----------

	Button(GameObject* in_gameObject);
	~Button();

	//	Public Function
	//	---------------

	void awake() override;
	void update() override;

	void showImGUI() override;
	void saveComponentInSCNFile(std::ofstream& file) override;
	void loadComponentFromSCNFile(std::istringstream& lineStream) override;

	//	Public Variables
	//	----------------
	bool m_disable = false;
	bool m_hovered = false;

	BoxCollider2D* m_collider = nullptr;
	Sprite* m_sprite = nullptr;

	ButtonAction* m_action = nullptr;
};