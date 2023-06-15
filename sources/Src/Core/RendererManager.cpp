#pragma once

#include <Config.hpp>
#include <Core/RendererManager.hpp>
#include <Core/Graph.hpp>

#include <Resources/ResourcesManager.hpp>
#include <Resources/Shader.hpp>

#include <Engine/GameObject.hpp>
#include <Engine/Layers.hpp>
#include <Engine/EditorManager.hpp>

#include <LowRenderer/Camera.hpp>
#include <LowRenderer/Sprite.hpp>
#include <LowRenderer/ParticleSystem.hpp>
#include <LowRenderer/SpriteBillboard.h>
#include <LowRenderer/Text.hpp>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>


Core::RendererManager::RendererManager()
{	if(Core::Graph::instance()->m_mode != EngineMode::FULLPLAYMODE) m_cameraList[0] = &m_editorCamera;
	m_activeCamera = 0;

	m_cubeMap = new LowRenderer::CubeMap("Assets/defaultCubeMap.cubemap");
}

Core::RendererManager::~RendererManager()
{
	m_lightList.clear();
	m_modelList.clear();
	m_cameraList.clear();

	delete m_cubeMap;
}

void Core::RendererManager::sendDatasToGPU(const CameraBase& activeCamera)
{
	Resources::ResourcesManager* resources = Resources::ResourcesManager::instance();


	//	Send datas to the Shaders
	//	-------------------------

	for (auto curr_shader : resources->m_shaderName_shader)
	{
		Resources::Shader* shad = &curr_shader.second;
		if (shad->m_type == Resources::ShaderType::POST_PROCESS) continue;

		shad->use();
		if (activeCamera.projectionMode == ORTHOGRAPHIC)
		{
			shad->setMat4("Projection", activeCamera.getOrthographicProjection());
		}
		else
		{
			shad->setMat4("Projection", activeCamera.getPerspectiveProjection());
		}

		shad->setMat4("View", activeCamera.getViewMatrix());

		if (shad->m_type == Resources::ShaderType::SHADER_3D)
		{
			shad->setLights(m_lightList);

			shad->setFloat3("ViewPos", activeCamera.getPosition());
		}
	}
}


void Core::RendererManager::update()
{
	if (getActiveCamera() == &m_editorCamera)
	{
		m_editorCamera.update();
	}
	m_UICamera.update();
}

void Core::RendererManager::draw()
{

	if (getActiveCamera() != nullptr)
	{
		//	Send datas to GPU before Drawing
		sendDatasToGPU(*getActiveCamera());
	}

	m_postProcess.beginRender();

	if (getActiveCamera() != nullptr)
	{
		if(m_cubeMap) m_cubeMap->draw();

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);

		//	Draw each models
		for (auto _model : m_modelList)
		{
			//	Verify if it still exist
			if (_model.second == nullptr)
			{
				m_modelList.erase(_model.first);
				continue;
			}

			if (_model.second->isActive()) _model.second->draw();
		}

		//	Draw each particle
		for (auto _particleSystem : m_particleSystemList)
		{
			//	Verify if it's still exist
			if (_particleSystem.second == nullptr)
			{
				m_particleSystemList.erase(_particleSystem.first);
				continue;
			}

			if (_particleSystem.second->isActive()) _particleSystem.second->draw();
		}

		//	Draw each billboarded sprite 
		for (auto billsprite : m_spriteBillboardList)
		{
			//	Verify if it's still exist
			if (billsprite.second == nullptr)
			{
				m_particleSystemList.erase(billsprite.first);
				continue;
			}

			if (billsprite.second->isActive()) billsprite.second->draw();
		}

		glDisable(GL_BLEND);
	}

	m_postProcess.endRender();
	m_postProcess.render();

	sendDatasToGPU(m_UICamera);
	glClear(GL_DEPTH_BUFFER_BIT);

	//	Draw each sprite
	for (auto& _sprite : m_spriteList)
	{
		GameObject* obj = nullptr;
		if (_sprite.second)
			obj = _sprite.second->m_gameObject;
		if (obj == nullptr)
			continue;

		if (obj->m_sceneReference->m_pause && obj->m_layer.layer == Layer::MaskOnPause) continue;

		//	Verify if it still exist
		if (_sprite.second == nullptr)
		{
			m_spriteList.erase(_sprite.first);
			continue;
		}

		if (_sprite.second->isActive()) _sprite.second->draw();
	}
	glDisable(GL_DEPTH_TEST);

	TextRender::instance()->RenderTextBuffer();
}


CameraBase* Core::RendererManager::getActiveCamera()
{
	if (m_cameraList.empty()) return nullptr;
	return m_cameraList[m_activeCamera];
}

void Core::RendererManager::showImGUIRendererManager()
{
	if (ImGui::CollapsingHeader("Cameras", ImGuiTreeNodeFlags_DefaultOpen))
	{
		int camera_number = (int)m_cameraList.size();
		ImGui::SliderInt(" <- Camera ID", &m_activeCamera, 0, camera_number - 1);

		if (camera_number > 1)
		{
			if (ImGui::Button("Remove Camera"))
			{
				m_cameraList.erase(m_activeCamera);

				m_activeCamera = 0;
			}
		}

		ImGui::PushID("Editor Camera");

		if (ImGui::CollapsingHeader("Editor Camera", ImGuiTreeNodeFlags_DefaultOpen))
		{
			m_editorCamera.showCameraImGUI();
		}
		ImGui::PopID();


		ImGui::PushID("HUD Camera");
		if (ImGui::CollapsingHeader("HUD Camera", ImGuiTreeNodeFlags_DefaultOpen))
		{
			m_UICamera.showHUDCameraImGui();
		}
		ImGui::PopID();
	}

	if (ImGui::CollapsingHeader("PostProcess", ImGuiTreeNodeFlags_DefaultOpen))
	{
		m_postProcess.showImGui();
	}
}
