#include <Core/TimeManager.h>
#include <Core/Log.hpp>

#include <string>

#include <Engine/Transform3.hpp>

#include <Physics/RigidBody3.hpp>
#include <Physics/Collider3.hpp>
#include <Physics/PhysicsManager.hpp>
using namespace Physics;

#include <Resources/Scene.hpp>

#include <Core/Graph.hpp>

#include <Maths/Utils.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>


Rigidbody3::Rigidbody3(GameObject* in_gameObject) : Component(in_gameObject)
{
	init(ComponentType::RigidBody);
	//m_transform->linkToRigidBody(this);

	Resources::Scene* scene = Core::Graph::instance()->getNextScene();
	if (scene)
		scene->m_physicsManager.registerRigidbody(this);
}

Rigidbody3::~Rigidbody3()
{
	Resources::Scene* scene = Core::Graph::instance()->getCurrentScene();
	if (scene)
		scene->m_physicsManager.unregisterRigidbody(this);

	Component::~Component();
}

void Rigidbody3::awake()
{
	m_collider = m_gameObject->getComponent<Collider3>();

	if (m_collider == nullptr)
		m_collider = m_gameObject->addComponent<SphereCollider3D>();
}


void Rigidbody3::Reset()
{
	m_velocity = { 0 };
}


void Rigidbody3::addForce(const Vector3f& force, ForceMode mode)
{
	switch (mode)
	{
	case ForceMode::Acceleration:
		m_forceAcceleration += force;
		break;
	case ForceMode::Impulse:
		m_forceImpulse += force;
		break;
	default:
		break;
	}

}


Vector3f Rigidbody3::calculateNextPos()
{
	Vector3f gravity_acceleration = { 0.f, -EARTH_GRAVITY * m_gravityMultiplier, 0.f };
	Vector3f drag_force = m_velocity.normalized() * m_velocity.squareLength() * m_drag * (-0.5f);

	m_acceleration = (m_forceAcceleration) / m_mass + gravity_acceleration;


	m_velocity += (m_acceleration + m_forceImpulse + drag_force) * Core::TimeManager::instance()->fixedDeltaTime;

	m_forceAcceleration = { 0 };
	m_forceImpulse = { 0 };

	return m_transform->m_position + m_velocity * Core::TimeManager::instance()->fixedDeltaTime;
}

void Rigidbody3::updatePosition()
{
	m_transform->m_position += m_velocity * Core::TimeManager::instance()->fixedDeltaTime;
}


void Rigidbody3::showImGUI()
{
	using namespace ImGui;

	DragFloat("Mass", &m_mass);
	DragFloat("Drag coefficient", &m_drag);
	if (Checkbox("Is Kinematic", &m_isKinematic))Reset();

	if (Checkbox("Use Gravity", &m_useGravity)) Reset();

	std::string v = "Velocity : " + std::to_string(m_velocity.x) + " , " + std::to_string(m_velocity.y) + " , " + std::to_string(m_velocity.z);
	Text(v.c_str());
}

void Rigidbody3::saveComponentInSCNFile(std::ofstream& file)
{
	file << "RIGIDBODY\t" << m_mass << "/" << m_drag << "/" << (int)m_isKinematic << "/" << (int)m_useGravity << "\n";
}

void Rigidbody3::loadComponentFromSCNFile(std::istringstream& lineStream)
{
	//	Write in log
	Core::Log* _log = Core::Log::instance();
	_log->write("+\t Adding light to new gameObject");

	m_mass = FileParser::getFloat(lineStream);
	lineStream.ignore();

	m_drag = FileParser::getFloat(lineStream);
	lineStream.ignore();

	m_isKinematic = FileParser::getInt(lineStream);
	lineStream.ignore();

	m_useGravity = FileParser::getInt(lineStream);
}
