#pragma once
#include <Engine/Component.hpp>

class GameObject;

/*
* Must Be parent of menu components
*/
class PauseMenu : public Component
{
public:

	//	Constructor
	//	-----------

	PauseMenu(GameObject* in_gameObject);

	//	Variables
	//	---------

	GameObject* menu = nullptr;

	//	Fucntions
	//	---------

	void awake() override;
	void update() override;
	void showImGUI() override;
	void saveComponentInSCNFile(std::ofstream& file) override;
	void loadComponentFromSCNFile(std::istringstream& lineStream) override;
};