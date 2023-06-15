#include <iostream>
#include <fstream>
#include <sstream>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <Core/Log.hpp>
#include <Core/TimeManager.h>
#include <Core/InputsManager.hpp>
#include <Core/Graph.hpp>
#include <Core/Window.hpp>

#include <LowRenderer/Text.hpp>
#include <Resources/ResourcesManager.hpp>
#include <Resources/Scene.hpp>

#include <Maths/Matrix.h>
#include <Engine/Transform3.hpp>
#include <Maths/Quaternion.h>

#include <Physics/RigidBody3.hpp>
#include <Physics/Collider3.hpp>

Resources::Scene::Scene()
{
	m_loader_transform.m_scale = { 3.f, 3.f, 3.f };
	m_loader_sprite.m_transform = &m_loader_transform;
}

Resources::Scene::~Scene()
{
	Core::Log* _log = Core::Log::instance();
	_log->write("- Unloading scene");

	//	Destroy all GameObjects
	while(m_objectList.empty() == false)
	{
		m_objectList[(int)m_objectList.size() - 1]->destroy();
	}
	m_objectList.clear();
}

//	Load .scn scene
//	---------------

bool Resources::Scene::loadScene(const char* path)
{
	Core::Log* _log = Core::Log::instance();
	Resources::ResourcesManager* resources = Resources::ResourcesManager::instance();

	m_physicsManager.initialize();

	//	Open file in read mode
	std::ifstream file(path);

	//	If failed return
	if (!file)
	{
		_log->writeError("Failed to open file \"" + std::string(path) + "\".");
		return false;
	}

	std::string curr_line;
	std::string curr_shader_name;

	while (std::getline(file, curr_line))
	{
		std::istringstream lineStream(curr_line);

		//	Get the type of the next values
		std::string type = FileParser::getString(lineStream);

		if (type[0] == '#' || curr_line[0] == '\n' || curr_line[0] == '\0') continue;

		//	Read & Load GameObject
		if (type == "GAMEOBJECT")
		{
			drawLoading();
			loadGameObject(file, lineStream, *this);
			_log->breakLine();

			continue;
		}
	}

	for (auto obj : m_objectList)
	{
		obj.second->birth();
	}

	_log->breakLine();
	file.close();

	m_physicsManager.setUp();

	return true;
}

void Resources::Scene::saveScene(const char* path)
{
	Core::Log* _log = Core::Log::instance();
	std::ofstream file(path);

	_log->write("Saving current scene in \"" + std::string(path) + "\"...");

	if (file.is_open() == false)
	{
		_log->writeFailure("Unable to open/create file (" + std::string(path) + ")");
		_log->writeFailure("Scene couldn't be saved");
		return;
	}

	Resources::ResourcesManager* resources = Resources::ResourcesManager::instance();

	for (auto obj : m_objectList)
	{
		GameObject* currGameObject = obj.second;

		file << "GAMEOBJECT\t";
		file << currGameObject->m_name << " ";
		file << currGameObject->m_isEnabled << " ";
		file << currGameObject->m_isStatic << " ";
		file << currGameObject->m_layer.name << "\n";

		for (auto comp : currGameObject->m_componentList)
		{
			comp.second->saveComponentInSCNFile(file);
		}

		file << "END\n\n";
	}

	file.close();

	_log->writeSuccess("Scene has been saved in \"" + std::string(path) + "\"");
}



void Resources::Scene::pause()
{
	Core::TimeManager* _time = Core::TimeManager::instance();

	if (m_pause)
	{
		_time->timeScale = 0.f;
	}
	else
	{
		_time->timeScale = 1.f;
	}
}


GameObject* Resources::Scene::findGameObjectInScene(std::string name)
{
	for (auto object : m_objectList)
	{
		if (object.second->m_name == name)
			return object.second;
	}
	return nullptr;
}

/*---------------------UPDATE--------------------*/

void Resources::Scene::gameLoop()
{
	Core::Graph* _graph = Core::Graph::instance();
	Core::TimeManager* _time = Core::TimeManager::instance();

	awake();
	start();

	update();

	_time->setFixedDeltaTime();
	while (_time->fixedDeltaTimeLoop())
	{
		fixedUpdate();
		m_physicsManager.processPhysics();
	}

	lateUpdate();
}

void Resources::Scene::editLoop()
{
	Core::Graph* _graph = Core::Graph::instance();
	Core::TimeManager* _time = Core::TimeManager::instance();

	update();
}


void Resources::Scene::awake()
{
	for (auto object : m_objectList)
	{
		object.second->awake();
	}
}

void Resources::Scene::start()
{
	for (auto object : m_objectList)
	{
		object.second->start();
	}	

	if (ambientStarted) return;

	if (Core::Graph::instance()->m_current_scene == "Assets/MainMenu.scn")
	{
		ambient_music = Core::Graph::instance()->AudioPlayer->play2D("Assets/audio/main_menu.wav", true); 
		ambientStarted = true;
	}

	if (Core::Graph::instance()->m_current_scene == "Assets/NewGame.scn" || Core::Graph::instance()->m_current_scene == "Assets/SavedGame.scn")
	{
		ambient_music = Core::Graph::instance()->AudioPlayer->play2D("Assets/audio/main_theme.wav", true);
		ambientStarted = true;
	}
}


void Resources::Scene::update()
{
	m_rendererManager.update();

	Core::Graph* _graph = Core::Graph::instance();
	if (_graph->m_mode != EngineMode::PLAYMODE && _graph->m_mode != EngineMode::FULLPLAYMODE) return;

	Core::InputsManager* _intputManager = Core::InputsManager::instance();

	if (_intputManager->m_key[PAUSE]->pressed)
	{
		m_pause = !m_pause;
		pause();
	}

	for (auto object : m_objectList)
	{
		object.second->update();
	}
}

void Resources::Scene::fixedUpdate()
{
	for (auto object : m_objectList)
	{
		object.second->fixedUpdate();
	}
}

void Resources::Scene::lateUpdate()
{
	for (auto object : m_objectList)
	{
		object.second->lateUpdate();
	}
}



void Resources::Scene::draw()
{
	for (auto object : m_objectList)
	{
		object.second->m_transform->updateTransform();
	}

	m_rendererManager.draw();
}

void Resources::Scene::drawLoading() 
{
	glClearColor(0.f, 0.f, 0.f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_loader_sprite.draw();
	static int pointNb = 0;
	pointNb = (pointNb < 30) ? pointNb + 1 : 0;
	int points = pointNb / 10;
	std::string loading = "LOADING";
	for (int i = 0; i < points; i++)
		loading += " .";

	TextRender::instance()->AddText("abnes.ttf", loading.c_str(), { -3, -5 }, .75f, { 1.f, 1.f, 1.f});
	TextRender::instance()->RenderTextBuffer();
	glfwSwapBuffers(Core::Window::instance()->m_window);
	m_loader_transform.m_rotation.z -= .1f;
	m_loader_transform.updateTransform();

}

//	ImGui of Scene
//	--------------

void Resources::Scene::showImGUIScene()
{
	static char savedFileName[128] = "Assets/NewGame.scn";
	ImGui::InputText("Name ", savedFileName, IM_ARRAYSIZE(savedFileName));

	if (ImGui::Button("Save"))
	{
		saveScene(savedFileName);
	}

	//	SCENE OBJECTS Parameters
	//	-----------------------

	if (ImGui::CollapsingHeader("Objects", ImGuiTreeNodeFlags_DefaultOpen))
	{
		for (int n = 0; n < (int)m_objectList.size(); n++)
		{
			bool is_selected = (m_objectList[n] == m_selectedGameObject);

			//ImGui::InputText(m_objectList[n]->m_name.c_str())

			char* objName = const_cast<char*>(m_objectList[n]->m_name.c_str());

			std::string ID = "###Object" + std::to_string(n);

			ImGui::PushID(ID.c_str());

			ImGui::InputText("###GameObjectRename", objName, 128);
			m_objectList[n]->m_name = std::string(objName);

			ImGui::SameLine();
			if (ImGui::Selectable("Select", is_selected))
			{
				m_selectedGameObject = m_objectList[n];
			}
			if (is_selected)
			{
				ImGui::SetItemDefaultFocus();
			}

			ImGui::PopID();
		}

		ImGui::NewLine();

		static char newObjectName[128] = "EmptyObject";
		ImGui::InputText("###NameNewGameObject", newObjectName, IM_ARRAYSIZE(newObjectName));


		if (ImGui::Button("Create new Game Object"))
		{
			m_objectList[(int)m_objectList.size()] = new GameObject(this);
			m_objectList[(int)m_objectList.size() - 1]->m_name = std::string(newObjectName);
		}
	}

	ResourcesManager::instance()->showImGUIResourcesManager();
	m_rendererManager.showImGUIRendererManager();
}


void Resources::Scene::showImGUIGameObject()
{
	if (m_selectedGameObject != nullptr) m_selectedGameObject->showImGUIGameObject();
}
