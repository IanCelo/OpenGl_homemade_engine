#include <iostream>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <LowRenderer/Light.hpp>
#include <Engine/GameObject.hpp>
#include <Resources/Scene.hpp>

#include <Core/Log.hpp>

Light::Light(GameObject* in_gameObject) : Component(in_gameObject)
{
	init(ComponentType::Light);

	m_gameObject->m_sceneReference->m_rendererManager.m_lightList[(int)m_gameObject->m_sceneReference->m_rendererManager.m_lightList.size()] = this;
}


void Light::showImGUI()
{
	using namespace ImGui;
	
	if (TreeNode("Light setting"))
	{
		Text("Colors");
		ColorEdit3("<- Ambient", &ambient.c[0]);
		ColorEdit3("<- Diffuse", &diffuse.c[0]);
		ColorEdit3("<- Specular", &specular.c[0]);

		Text("Attenuation");
		SliderFloat("<- Constant", &attenuation.c[0], 0.f, 50.f);
		SliderFloat("<- Linear", &attenuation.c[1], 0.f, 10.f);
		SliderFloat("<- Quadratic", &attenuation.c[2], 0.f, 1.f);

		Text("Type & Power");
		SliderFloat("<- Power", &power, 0.f, 100.f);
		SliderInt(" <- Light Type", &lightType, 0, 2);

		if (lightType > 0)
		{
			SliderFloat3("<- Direction", &direction.c[0], -1.f, 1.f);

			if (lightType == (int)LightType::SPOT_LIGHT)
			{
				SliderFloat("<- Cut Off", &cutOff, 0.f, 1.f);
				SliderFloat("<- Outer Cut Off", &outerCutOff, 0.f, 1.f);
			}
		}

		TreePop();
	}
}


void Light::destroy()
{
	//  get Renderer Manager of the scene
	Core::RendererManager* _renderer = &m_gameObject->m_sceneReference->m_rendererManager;

	//  Get last index (trash index)
	int lastIndex = (int)_renderer->m_lightList.size() - 1;

	//  Parsing list of the current renderer component type
	for (auto comp : _renderer->m_lightList)
	{
		//  Once we find "this", let's procees to the retirement of the component from the list
		if (comp.second == this)
		{
			//  Swap current with the last one if it's not already the last one
			if (comp.first != lastIndex)
			{
				_renderer->m_lightList[comp.first] = _renderer->m_lightList[lastIndex];
			}

			//  Set the pointer to null
			_renderer->m_lightList[lastIndex] = nullptr;

			//  Then erase it from the list
			_renderer->m_lightList.erase(lastIndex);

			//  And leave the loop
			//  To avoid errors and optimize
			break;
		}
	}

	//  Then call the common component destoy function
	Component::destroy();
}


void Light::saveComponentInSCNFile(std::ofstream& file)
{
	file << "LIGHT\t\t";

	FileWriter::writeVec3InFile(file, ambient); file << "/";
	FileWriter::writeVec3InFile(file, diffuse); file << "/";
	FileWriter::writeVec3InFile(file, specular); file << "/";
	FileWriter::writeVec3InFile(file, attenuation); file << "/";
	FileWriter::writeVec3InFile(file, direction); file << "/";

	file << power << "/" << cutOff << "/" << outerCutOff << "/" << lightType % 2 << "\n";
}

void Light::loadComponentFromSCNFile(std::istringstream& lineStream)
{
	//	Write in log
	Core::Log* _log = Core::Log::instance();
	_log->write("+\t Adding light to new gameObject");

	ambient = FileParser::getVector3(lineStream);
	lineStream.ignore();
	diffuse = FileParser::getVector3(lineStream);
	lineStream.ignore();
	specular = FileParser::getVector3(lineStream);
	lineStream.ignore();
	attenuation = FileParser::getVector3(lineStream);
	lineStream.ignore();
	direction = FileParser::getVector3(lineStream);

	lineStream.ignore();
	power = FileParser::getFloat(lineStream);
	lineStream.ignore();
	cutOff = FileParser::getFloat(lineStream);
	lineStream.ignore();
	outerCutOff = FileParser::getFloat(lineStream);
	lineStream.ignore();
	lightType = FileParser::getInt(lineStream) % 2;
}