#include <Scripts/PauseMenu.hpp>
#include <Scripts/Button.hpp>
#include <LowRenderer/Sprite.hpp>

#include <Core/InputsManager.hpp>
#include <Core/Graph.hpp>

#include <Engine/GameObject.hpp>
#include <Engine/Transform3.hpp>
#include <Config.hpp>

#include <Resources/Scene.hpp>

PauseMenu::PauseMenu(GameObject* in_gameObject) : Component(in_gameObject)
{
	init(ComponentType::PauseMenu);
}

void PauseMenu::awake()
{
}

void PauseMenu::update()
{
	Core::InputsManager* _inputs = Core::InputsManager::instance();

	//	Is the game paused
	if (m_gameObject->m_sceneReference->m_pause)
	{
		//	If true

		//	Display mouse so the user can interact with the pause menu
		_inputs->displayMouse();

		//	And Enable children so the menu can show up
		for (Transform3* t : m_transform->m_childList)
		{
			t->m_gameObject->m_isEnabled = true;
		}
	}
	else
	{
		//	Else

		//	If the user isn't in EDITMODE, hide the mouse
		if(Core::Graph::instance()->m_mode != EngineMode::EDITMODE) _inputs->hideMouse();

		//	And Disable children to hide pause menu
		for (Transform3* t : m_transform->m_childList)
		{
			t->m_gameObject->m_isEnabled = false;
		}
	}
}

void PauseMenu::showImGUI()
{
};

void PauseMenu::saveComponentInSCNFile(std::ofstream& file)
{
	file << "PAUSEMENU\n" ;
};

void PauseMenu::loadComponentFromSCNFile(std::istringstream& lineStream)
{

}