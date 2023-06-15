
#pragma once

#include "Resources/ResourcesManager.hpp"
#include "Resources/Scene.hpp"

#include <IK/irrKlang.h>

enum class EngineMode
{
	FULLPLAYMODE,
	PLAYMODE,
	EDITMODE,
};

namespace Core
{
	class Graph : public Singleton<Graph>
	{
	private:
		//	Private Variables
		//	-----------------

		//	List of scenes
		std::unordered_map<std::string,  Resources::Scene*> m_sceneList;

	public:
		//	Constructors
		//	------------

		Graph();
		~Graph();

		//	Public Variables
		//	-----------------

		irrklang::ISoundEngine* AudioPlayer;

		//	Current scene Index
		std::string m_current_scene = "None";

		bool m_quit = false;
		bool m_unload = false;
		EngineMode  m_mode = EngineMode::EDITMODE;
		std::string m_nextScene;

		//	Internal Functions
		//	------------------

		//	Initialize graph
		//	-----------------
		void initialize();

		//	Return current Camera in the following index
		//	-----------------
		CameraBase* getCurrentCamera();

		//	Scene load & unload
		//	-----------------
		void newScene(const std::string name);
		bool loadScene(const std::string& path);
		void unloadCurrentScene();
		void switchToScene(const std::string& name);

		//	Scene management
		//	-----------------
		void graphLoop();

		void drawCurrentScene();
		void updateCurrentScene();

		//	Useful functions
		//	-----------------
		Resources::Scene* getCurrentScene();
		Resources::Scene* getNextScene();
		bool isSceneListEmpty();


		//	Show ImGui graph window
		//	-----------------
		void showImGUIGraph();
		void showImGUIScene();
		void showImGUIResourcesManager();
		void showImGUIGameObject();
	};
}