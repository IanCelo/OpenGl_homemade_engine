#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <Core/Log.hpp>
#include <Core/TimeManager.h>
#include <Core/Graph.hpp>


#include <Engine/Layers.hpp>
#include <Utils/StringExtractor.h>

#include <Config.hpp>

Core::Graph::Graph()
{
	ComponentTypeInfo* _componentManager = ComponentTypeInfo::instance();
	LayerDatas* _layers = LayerDatas::instance();
	AudioPlayer = irrklang::createIrrKlangDevice();
}

Core::Graph::~Graph()
{
	Core::Log* _log = Core::Log::instance();
	_log->write("- Destroying Graph");

	ComponentTypeInfo* _componentManager = ComponentTypeInfo::instance();
	LayerDatas* _layers = LayerDatas::instance();

	_componentManager->kill();
	_layers->kill();

	for (auto scn : m_sceneList)
	{
		if (scn.second != nullptr) delete scn.second;
	}
	m_sceneList.clear();
}

void Core::Graph::initialize()
{
	//	Create ResourceManager Instance if it doesn't exist yet
	Resources::ResourcesManager* _resources = Resources::ResourcesManager::instance();

	_resources->initialize();
}

void Core::Graph::newScene(const std::string name)
{
	m_sceneList[name] = new Resources::Scene();
	m_nextScene = name;
	m_current_scene = name;
}

bool Core::Graph::loadScene(const std::string& path)
{
	Core::Log* _log = Core::Log::instance();

	std::string name = path;
	while (m_sceneList.find(name) != m_sceneList.end())
	{
		std::string nameWithoutExtension = Extractor::ExtractFilename(Extractor::ExtractNameWithoutExtension(name));

		int num = Extractor::ExtractNumberAtEnd(nameWithoutExtension);
		num++;
		name = Extractor::ExtractDirectory(name) + nameWithoutExtension + std::to_string(num) + ".scn";
	}

	m_nextScene = name;

	//	Add the new scene in the list
	m_sceneList[name] = new Resources::Scene();


	//	If the scene failed to load
	if (m_sceneList[name]->loadScene(path.c_str()) == false)
	{
		delete m_sceneList[name];

		_log->writeError("Failed to load scene");
		_log->breakLine();

		//	Remove the failure scene
		m_sceneList.erase(name);

		return false;
	}
	//	Esle if the scene succeded in loading
	
	//	Change the current scene index to the new one
	m_current_scene = name;

	_log->writeSuccess("Successfully loaded a new scene");
	_log->breakLine();

	return true;
}


void Core::Graph::unloadCurrentScene()
{
	Core::Log* _log = Core::Log::instance();

	_log->write("Unloading a scene");

	//	Erase the current scene from the list
	delete m_sceneList[m_current_scene];
	m_sceneList.erase(m_current_scene);

	//	Then Change scene index to the previous one
	m_current_scene = "None";
}


void Core::Graph::graphLoop()
{
	updateCurrentScene();

	drawCurrentScene();
}

void Core::Graph::drawCurrentScene()
{
	//	If scene list is empty, return
	if (m_sceneList.empty()) return;

	//	If current list index is valide draw the current scene
	if (m_sceneList[m_current_scene])
	{
		m_sceneList[m_current_scene]->draw();
	}
}

void Core::Graph::updateCurrentScene()
{
	Core::TimeManager* _time = Core::TimeManager::instance();

	//	If scene list is empty, return
	if (m_sceneList.empty()) return;

	if(m_nextScene != m_current_scene)
	{
		if (getCurrentScene())
			getCurrentScene()->ambientStarted = false;
		std::string oldScene = m_current_scene;

		if (m_unload)
		{
			m_unload = false;
			unloadCurrentScene();
		}

		//	If scene was not found
		if (m_sceneList.find(m_nextScene) == m_sceneList.end())
		{
			//	Load a new scene
			if (loadScene(m_nextScene) == false)
			{
				m_nextScene = oldScene;
				m_current_scene = oldScene;
			}
		}
		m_current_scene = m_nextScene;
		AudioPlayer->stopAllSounds();
	}

	//	If current list index is valide update the current scene
	if (m_sceneList[m_current_scene])
	{
		switch (m_mode)
		{
		case EngineMode::EDITMODE:
			m_sceneList[m_current_scene]->editLoop();
			break;
		case EngineMode::PLAYMODE:
			m_sceneList[m_current_scene]->gameLoop();
			break;
		case EngineMode::FULLPLAYMODE:
			m_sceneList[m_current_scene]->gameLoop();
			break;
		default:
			break;
		}
	}
}


bool Core::Graph::isSceneListEmpty()
{
	return m_sceneList.empty();
}

void Core::Graph::switchToScene(const std::string& name)
{
	m_nextScene = name;
}

Resources::Scene* Core::Graph::getCurrentScene()
{
	//	Return Current Scene
	//	--------------------
	return m_sceneList[m_current_scene];
}

Resources::Scene* Core::Graph::getNextScene()
{
	//	Return Current Scene
	//	--------------------
	return m_sceneList[m_nextScene];
}

CameraBase* Core::Graph::getCurrentCamera()
{
	//	Return Current Camera
	//	---------------------
	return m_sceneList[m_current_scene]->m_rendererManager.getActiveCamera();
}


void Core::Graph::showImGUIGraph()
{
	//	get log instance
	Core::Log* _log = Core::Log::instance();

	using namespace ImGui;

	//	Play / Edit Mode buttons
	{
		Resources::Scene* _currentScene = getCurrentScene();
		Core::TimeManager* _time = Core::TimeManager::instance();

		if (m_mode == EngineMode::EDITMODE)
		{
			if (Button("PlayMode"))
				m_mode = EngineMode::PLAYMODE;
		}
		else if (m_mode == EngineMode::PLAYMODE)
		{
			if (Button("EditMode"))
				m_mode = EngineMode::EDITMODE;
		}
	}


	if (CollapsingHeader("Scenes", ImGuiTreeNodeFlags_DefaultOpen))
	{
		//	Load a scene
		//	------------

		static char path[128] = "Assets/";
		static char name[128] = "MainMenu";
		InputText("Scene path", path, IM_ARRAYSIZE(path));
		InputText("Scene name", name, IM_ARRAYSIZE(name));

		// New scene
		// ---------
		if (Button("New Empty Scene"))
		{
			newScene(name);
		}

		//	Load Button is clicked
		if (Button("Load"))
		{
			loadScene((std::string)path + (std::string)name + ".scn");
		}

		//	Unload a scene
		//	--------------

		//	If scene list isn't empty
		if (m_sceneList.empty() == false)
		{
			//	If Unload Button is clicked
			if (Button("Unload"))
			{
				Text("Unloading file...");

				_log->write("Unloading a scene");

				//	Erase the current scene from the list
				delete m_sceneList[m_current_scene];
				m_sceneList.erase(m_current_scene);

				//	Then Change scene index to the previous one
				m_current_scene = "None";
			}
		}
	}

	//	If Scene list is not empty
	if (m_sceneList.empty() == false)
	{
		std::string preview = m_current_scene;

		if (ImGui::BeginCombo("Choose Scene", preview.c_str()))
		{
			if (preview != "None")
			{
				if (ImGui::Selectable("None"))
				{
					switchToScene("None");
					m_current_scene = "None";
				}
			}
			

			for (auto scn : m_sceneList)
			{
				if(ImGui::Selectable(scn.first.c_str()))
				{
					switchToScene(scn.first);
				}
			}

			ImGui::EndCombo();
		}
	}
}

void Core::Graph::showImGUIScene()
{
	if (m_sceneList[m_current_scene])
	{
		m_sceneList[m_current_scene]->showImGUIScene();
	}
}

void Core::Graph::showImGUIResourcesManager()
{
	Resources::ResourcesManager::instance()->showImGuiWindows();
}

void Core::Graph::showImGUIGameObject()
{
	if (m_sceneList[m_current_scene])
	{
		m_sceneList[m_current_scene]->showImGUIGameObject();
	}
}