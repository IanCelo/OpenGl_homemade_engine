#pragma once

#include <Engine/Component.hpp>
#include <Engine/Layers.hpp>
#include <Engine/Transform3.hpp>

#include <vector>
#include <memory>
#include <unordered_map>

#include <string>
#include <fstream>
#include <sstream>

//	Forward declarations
//	--------------------

class Model;
//class Transform3;

namespace Resources
{
	class Scene;
}


class GameObject
{
private:

	//	Private Internal Functions
	//	--------------------------

	template<typename C, typename Requires = std::enable_if_t<std::is_base_of<Component, C>::value>>
	bool tryGetComponentIndex(int& i)
	{
		for (i = 0; i < (int)m_componentList.size(); i++)
		{
			if (m_componentList[i] == nullptr) continue;

			if (dynamic_cast<C*>(m_componentList[i]) != nullptr)
			{
				return true;
			}
		}
		return false;
	}

	//	Check if a unique component is already in the component list
	//	Parameters : int i
	//	------------------
	bool HasUniqueComponentOfType(const int i);


	//	Check if a unique component is already in the component list
	//	Parameters : int i
	//	------------------
	void addComponentOfType(const int i);

	void loadOBJModelFromStringStream(std::istringstream& lineStream);
	void loadSingleModelFromStringStream(std::istringstream& lineStream);

	bool manuallyDisabled = false;
	int m_key;

public:
	//	Construcor
	//	----------

	GameObject();
	GameObject(Resources::Scene* in_sceneReference);

	~GameObject();

	//	Public Internal Variables
	//	-------------------------

	LayerData m_layer;

	std::string m_name;
	std::unordered_map<int, Component*> m_componentList;

	bool m_isEnabled = true;
	bool m_isStatic = false;

	Transform3* m_transform = nullptr;

	Resources::Scene* m_sceneReference;

	//	Public Internal Functions
	//	-------------------------

	//	Add Component, and return the new component
	//	Parameters : none
	//  -----------------
	template<typename C, typename Requires = std::enable_if_t<std::is_base_of<Component, C>::value>>
	C* addComponent()
	{
		int i;

		if (tryGetComponentIndex<C>(i))
		{
			ComponentTypeInfo* _componentManager = ComponentTypeInfo::instance();

			//	If is a unique component return
			if (_componentManager->m_componentList[m_componentList[i]->m_type].isUnique) return nullptr;
		}
		int index = (int)m_componentList.size();
		m_componentList[index] = new C(this);

		return dynamic_cast<C*>(m_componentList[index]);
	}

	//	Return true if successfully got the expected component and out is the pointer of the found component
	//	Parameters : C** out
	//	--------------------
	template<typename C, typename Requires = std::enable_if_t<std::is_base_of<Component, C>::value>>
	bool tryGetComponent(C** out)
	{
		for (auto comp : m_componentList)
		{
			*out = dynamic_cast<C*>(comp.second);

			if (*out) return true;
			
		}

		*out = nullptr;

		return false;
	}

	//	Return the awaited component 
	//	Be sure that the gameObject contain this component at least once
	//	Parameters : none
	//  -----------------
	template<typename C, typename Requires = std::enable_if_t<std::is_base_of<Component, C>::value>>
	C* getComponent()
	{
		int i;

		if (tryGetComponentIndex<C>(i))
		{
			return dynamic_cast<C*>(m_componentList[i]);
		}

		return nullptr;
	}

	//	If the current gameObject doesn't have any component of type C,
	//	It will add this component, and return it
	//	Parameters : none
	//	-----------------
	template<typename C, typename Requires = std::enable_if_t<std::is_base_of<Component, C>::value>>
	C* requireComponent()
	{
		int i;
		if (tryGetComponentIndex<C>(i)) return dynamic_cast<C*>(m_componentList[i]);
		
		return addComponent<C>();
	}

	//	Will return true if a component of type C was found in its children, 
	//	The pointer will point the found component
	//	Parameters : C** out
	//	--------------------
	template<typename C, typename Requires = std::enable_if_t<std::is_base_of<Component, C>::value>>
	bool tryGetComponentInChildren(C** out)
	{
		for (auto child : m_transform->m_childList)
		{
			if (child->m_gameObject->tryGetComponent<C>(out))
				return true;
		}

		return false;
	}

	void showImGUIGameObject();

	void loadFromScnFile(std::ifstream& file, Resources::Scene& m_scene);

	void birth();
	void awake();
	void start();
	void update();
	void fixedUpdate();
	void lateUpdate();
	void destroy();
	bool isActive();
	std::string getGameObjectID() const;
};

void loadGameObject(std::ifstream& file, std::istringstream& lineStream, Resources::Scene& m_scene);
