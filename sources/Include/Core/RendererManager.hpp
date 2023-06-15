#pragma once

#include <unordered_map>
#include <LowRenderer/CameraEditor.hpp>
#include <LowRenderer/CameraHUD.hpp>
#include <LowRenderer/PostProcessor.hpp>
#include <LowRenderer/CubeMap.hpp>

class Light;
class Camera;
class Model;
class Sprite;
class ParticleSystem;
class SpriteBillboard;

namespace Core
{
	class RendererManager
	{
	private:

		//	Private Internal Variables
		//	--------------------------

		int m_activeCamera;

		//	Send datas to all shaders
		//	Parameters : None
		//	-----------------
		void sendDatasToGPU(const CameraBase& activeCamera);

	public:

		//	Constructor & Destructor
		//	------------------------

		RendererManager();
		~RendererManager();


		//	Public Internal Variables
		//	-------------------------

		LowRenderer::CubeMap* m_cubeMap = nullptr;

		CameraEditor m_editorCamera;
		CameraHUD m_UICamera;

		PostProcessor m_postProcess;

		std::unordered_map<int, const Light*> m_lightList;
		std::unordered_map<int, CameraBase*> m_cameraList;
		std::unordered_map<int, Model*> m_modelList;
		std::unordered_map<int, Sprite*> m_spriteList;
		std::unordered_map<int, ParticleSystem*> m_particleSystemList;
		std::unordered_map<int, SpriteBillboard* > m_spriteBillboardList;

		//	Public Internal Functions
		//	-------------------------

		//	update editor camera
		//	Parameters : None
		//	-----------------
		void update();

		//	Draw saved models
		//	Parameters : None
		//	-----------------
		void draw();


		//	Show ImGui
		//	Parameters : None
		//	-----------------
		void showImGUIRendererManager();


		//	Get Active Camera
		//	Parameters : None
		//	-----------------
		CameraBase* getActiveCamera();
	};
}