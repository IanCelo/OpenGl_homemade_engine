#pragma once

#include <Utils/Singleton.h>
#include <Utils/File.h>
#include <unordered_map>

class GameObject;
class Transform3;
namespace Physics
{
	class Collider3;
}
using namespace Physics;

enum class ComponentType
{
	Transform,

	//	Low Renderer

	Model,
	Camera,
	Light,
	Sprite,
	ParticleSystem,
	SpriteBillboard,
	//	Script Component

	RigidBody,
	SphereCollider3D,
	BoxCollider3D,

	BoxCollider2D,

	Weapon,

	Player,
	Enemy,
	Button,
	PauseMenu,

	COUNT,
};


class ComponentTypeInfo : public Singleton<ComponentTypeInfo>
{
public:

	struct ComponentInfo
	{
		std::string name;
		bool isUnique = false;
		bool canDisable = false;
		bool canDestroy = true;
	};

	ComponentTypeInfo();
	std::unordered_map<int, ComponentInfo> m_componentList;
};

class Component
{

private:

	//	Private Variables
	//	-----------------

	bool m_manuallyDisabled = false;
	int m_key = 0;


protected:

	//	Protected Functions
	//	-------------------

	void init(ComponentType type);

public:

	//	Constructors & Destructors
	//	--------------------------

	Component() = default;
	Component(GameObject * in_gameObject);
	virtual ~Component();

	//	Public Internal Variables
	//	-------------------------

	GameObject* m_gameObject = nullptr;
	Transform3* m_transform  = nullptr;

	std::string m_name = "";

	int m_type = -1;

	bool m_isEnabled = true;
	bool m_hasAwaken = false;
	bool m_hasStarted = false;

	//	Public Internal Functions
	//	-------------------------

	//	First function called after scene load
	//	Pareameters: none
	//	-----------------
	virtual void birth() {};


	//	Second function called after birth load
	//	Pareameters: none
	//	-----------------
	virtual void awake() {};

	virtual void start() {};
	virtual void update() {};
	virtual void lateUpdate() {};
	virtual void fixedUpdate() {};

	virtual void onTriggerEnter(Collider3* collider) {};
	virtual void onTriggerStay(Collider3* collider) {};
	virtual void onTriggerExit(Collider3* collider) {};

	bool isActive() const;


	void showImGUIComponent();

	virtual void destroy();
	virtual void showImGUI() = 0;
	virtual void saveComponentInSCNFile(std::ofstream& file) = 0;
	virtual void loadComponentFromSCNFile(std::istringstream& lineStream) = 0;
};
