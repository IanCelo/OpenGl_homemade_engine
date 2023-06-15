#include <Scripts/Button.hpp>
#include <Scripts/ButtonPlayNewGame.hpp>
#include <Scripts/ButtonEditNewGame.hpp>
#include <Scripts/ButtonQuit.hpp>
#include <Scripts/ButtonLoad.hpp>
#include <Scripts/ButtonResume.hpp>
#include <Scripts/ButtonSaveGame.hpp>
#include <Scripts/ButtonBackToMainMenu.hpp>

#include <Engine/GameObject.hpp>
#include <Engine/Transform3.hpp>

#include <Core/InputsManager.hpp>
#include <Core/Graph.hpp>
#include <Core/Window.hpp>
#include <Core/Log.hpp>
#include <Maths/Utils.h>
#include <Physics/Collider2.hpp>
#include <LowRenderer/Sprite.hpp>
#include <iostream>
#include <Config.hpp>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>


Button::Button(GameObject* in_gameObject) : Component(in_gameObject)
{
	init(ComponentType::Button);
}

Button::~Button()
{
	if (m_action) delete m_action;
}

void Button::awake()
{
	//	Get Required Components

	m_gameObject->requireComponent<BoxCollider2D>();
	m_collider = m_gameObject->getComponent<BoxCollider2D>();

	m_gameObject->requireComponent<Sprite>();
	m_sprite = m_gameObject->getComponent<Sprite>();
}

void Button::update()
{
	Core::Window* _window		 = Core::Window::instance();
	Core::InputsManager* _inputs = Core::InputsManager::instance();

	Maths::Vector2f mousePos = _inputs->getMousePosition();
	
	float scale = ORTHOGRAPHIC_SCALE;

	//	Get mouse position in the HUD referential
	//	Clamp screen coordinates to HU coordinates [-10,10]
	Maths::Vector2f mousePosMapped = Maths::Vector2f
	(
		Maths::map(mousePos.x, 0.f, _window->m_widthf, -scale, scale), 
		Maths::map(mousePos.y, 0.f, _window->m_heightf, -scale, scale)
	);

	if (m_collider->IntersectWithPoint(mousePosMapped))
	{
		Core::Graph* _graph = Core::Graph::instance();
		
		//	Wasn't hovered before
		if (m_hovered == false)
		{
			//	Play sound
			_graph->AudioPlayer->play2D("Assets/audio/buttonHovered.wav", false);
		}

		//	The button is currently hovered
		m_hovered = true;

		//	Change color of the button, don't worry the default color has been saved
		m_sprite->m_color = m_sprite->m_color - Vector3f(0.5f);

		//	Is pressed
		if (_inputs->m_key[LEFT_CLICK]->pressed)
		{
			//	Play sound
			_graph->AudioPlayer->play2D("Assets/audio/buttonClicked.wav", false);

			//	Call action function
			if(m_action) m_action->action();
		}
	}
	else
	{
		//	The button is not hovered
		m_hovered = false;
	}
}

void Button::showImGUI()
{

	ImGui::NewLine();

	std::string preview = m_action ? m_action->m_name : "None";
	if (ImGui::BeginCombo("Action", preview.c_str()))
	{
		if (ImGui::Selectable("None"))
		{
			if (m_action) delete m_action;
			m_action = nullptr;
		}
		if (ImGui::Selectable("ButtonPlayNewGame"))
		{
			if (m_action) delete m_action;
			m_action = new ButtonPlayNewGame();
			m_action->m_button = this;
		}
		else if (ImGui::Selectable("ButtonEditNewGame"))
		{
			if (m_action) delete m_action;
			m_action = new ButtonEditNewGame();
			m_action->m_button = this;
		}
		if (ImGui::Selectable("ButtonLoad"))
		{
			if (m_action) delete m_action;
			m_action = new ButtonLoad();
			m_action->m_button = this;
		}
		else if (ImGui::Selectable("ButtonQuit"))
		{
			if (m_action) delete m_action;
			m_action = new ButtonQuit();
			m_action->m_button = this;
		}
		if (ImGui::Selectable("ButtonResume"))
		{
			if (m_action) delete m_action;
			m_action = new ButtonResume();
			m_action->m_button = this;
		}
		if (ImGui::Selectable("ButtonSaveGame"))
		{
			if (m_action) delete m_action;
			m_action = new ButtonSaveGame();
			m_action->m_button = this;
		}
		else if (ImGui::Selectable("ButtonBackToMainMenu"))
		{
			if (m_action) delete m_action;
			m_action = new ButtonBackToMainMenu();
			m_action->m_button = this;
		}
		
		ImGui::EndCombo();
	}
}

void Button::saveComponentInSCNFile(std::ofstream& file)
{
	file << "BUTTON\t\t" << m_action->m_name << "\n";
}

void Button::loadComponentFromSCNFile(std::istringstream& lineStream)
{
	//	Write in log
	Core::Log* _log = Core::Log::instance();
	_log->write("+\t Adding button script to new gameObject");

	std::string type = FileParser::getString(lineStream);

	if (type == "ButtonPlayNewGame")
	{
		m_action = new ButtonPlayNewGame();
		m_action->m_button = this;
	}
	else if (type == "ButtonEditNewGame")
	{
		m_action = new ButtonEditNewGame();
		m_action->m_button = this;
	}
	else if (type == "ButtonQuit")
	{
		m_action = new ButtonQuit();
		m_action->m_button = this;
	}
	else if (type == "ButtonLoad")
	{
		m_action = new ButtonLoad();
		m_action->m_button = this;
	}
	else if (type == "ButtonSaveGame")
	{
		m_action = new ButtonSaveGame();
		m_action->m_button = this;
	}
	else if (type == "ButtonResume")
	{
		m_action = new ButtonResume();
		m_action->m_button = this;
	}
	else if (type == "ButtonBackToMainMenu")
	{
		m_action = new ButtonBackToMainMenu();
		m_action->m_button = this;
	}
	else
	{
		m_action = nullptr;
	}
}