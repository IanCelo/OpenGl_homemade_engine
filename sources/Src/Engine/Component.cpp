#include <Engine/Component.hpp>
#include <Engine/Transform3.hpp>
#include <Engine/GameObject.hpp>

#include <Core/Log.hpp>

#include <Physics/Collider3.hpp>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

//	Datas from component types
ComponentTypeInfo::ComponentTypeInfo()
{
	m_componentList[(int)ComponentType::Transform] = { "Transform", true, false, false };

	//	LOWRENDERER COMPONENTS

	m_componentList[(int)ComponentType::Model] = { "Model", true, true, true };
	m_componentList[(int)ComponentType::Camera] = { "Camera", true, true, true };
	m_componentList[(int)ComponentType::Light] = { "Light", true, true, true };
	m_componentList[(int)ComponentType::Sprite] = { "Sprite", true, true, true };
	m_componentList[(int)ComponentType::ParticleSystem] = { "Particle System", false, true, true };
	m_componentList[(int)ComponentType::SpriteBillboard] = { "Sprite Billboard", false, true, true };
	
	//	PHYSICS COMPONENTS

	m_componentList[(int)ComponentType::RigidBody] = { "RigidBody 3D", true, false, true };
	m_componentList[(int)ComponentType::SphereCollider3D] = { "Sphere Collider 3D", false , true, true };
	m_componentList[(int)ComponentType::BoxCollider3D] = { "Box Collider 3D", false , true, true };

	m_componentList[(int)ComponentType::BoxCollider2D] = { "Box Collider 2D", true , true, true };

	//	GAMEPLAY SCRIPTS COMPONENTS

	m_componentList[(int)ComponentType::Weapon] = { "Weapon", false, true, true };
	m_componentList[(int)ComponentType::Player] = { "Player", false , true, true };
	m_componentList[(int)ComponentType::Enemy] = { "Enemy", false, true, true };

	m_componentList[(int)ComponentType::Button] = { "Button", false , true, true };
	m_componentList[(int)ComponentType::PauseMenu] = { "PauseMenu", false , true, true };
}

Component::~Component()
{
	//	Log writing
	Core::Log* _log = Core::Log::instance();
	_log->write("-\t\tDestroying Component " + m_name);
}

Component::Component(GameObject* in_gameObject)
{
	//	Set the owner (gameObject)
	m_gameObject = in_gameObject;

	//	Set key of the current component in the gameObject component list
	//	The key is not equal to "size - 1" because this index has not been created yet 
	m_key = (int)m_gameObject->m_componentList.size();

	//	If game object has a transform, set transform pointer of this component
	//	Normally all component should enter this scope, except for transform component
	if (in_gameObject->m_transform != nullptr)
	{
		m_transform = in_gameObject->m_transform;
	}
}

void Component::init(ComponentType type)
{
	//	Initialisation of the component

	//	Defining type of the component
	m_type = (int)type;

	//	Get components type datas manager
	ComponentTypeInfo* _componentManager = ComponentTypeInfo::instance();

	//	Get datas depending on its type
	m_name = _componentManager->m_componentList[(int)m_type].name;
}


bool Component::isActive() const
{
	//	Return if the component is active
	//	Will be inactive if parent objects are disabled
	return m_isEnabled && m_gameObject->isActive();
}


void Component::destroy()
{
	//	Get last index (trash index)
	int lastComp = (int)m_gameObject->m_componentList.size() - 1;

	//	Swap positions in the list
	m_gameObject->m_componentList[m_key] = m_gameObject->m_componentList[lastComp];
	m_gameObject->m_componentList[m_key]->m_key = m_key;


	//	Delete last component
	m_gameObject->m_componentList[lastComp] = nullptr;
	m_gameObject->m_componentList.erase(lastComp);

	//	Delete self
	delete this;
}


void Component::showImGUIComponent()
{
	if (ImGui::CollapsingHeader(m_name.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
	{
		ComponentTypeInfo* _componentManager = ComponentTypeInfo::instance();

		//	Only if the component can be disabled
		if (_componentManager->m_componentList[m_type].canDisable)
		{
			//	Endable checkbox
			if (ImGui::Checkbox("Enable Component", &m_isEnabled))
			{
				//	Switch active mode
				m_manuallyDisabled = !m_isEnabled;
			}
		}

		//	Show specifics parameters of components
		showImGUI();

		if (_componentManager->m_componentList[m_type].canDestroy && ImGui::Button("Delete component"))
		{
			destroy();
			return;
		}
	}
}
