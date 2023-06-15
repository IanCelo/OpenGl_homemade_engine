#include <Engine/Transform3.hpp>
#include <Engine/GameObject.hpp>

#include <Physics/Collider3.hpp>
#include <Physics/Rigidbody3.hpp>
#include <Physics/PhysicsManager.hpp>
using namespace Physics;

#include <Maths/Intersection3.h>
#include <Maths/Referential3.h>
#include <Maths/Utils.h>
#include <Maths/Quaternion.h>

#include <Resources/Scene.hpp>

#include <Scripts/Player.hpp>

#include <Core/Graph.hpp>
#include <Core/Log.hpp>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>


Collider3::Collider3(GameObject* in_gameObject) : Component(in_gameObject)
{

}

Collider3::~Collider3()
{
	Component::~Component();
}


Vector3f Collider3::getParentScaleMultiplier()
{
	const Transform3* parent = m_transform->m_parent;

	Vector3f parentScale = m_transform->m_scale;

	while (parent)
	{
		parentScale = Vector3f(parent->m_scale.x * parentScale.x,
			parent->m_scale.y * parentScale.y,
			parent->m_scale.z * parentScale.z);
	}

	return parentScale;
}

SphereCollider3D::SphereCollider3D(GameObject* in_gameObject) : Collider3(in_gameObject)
{
	init(ComponentType::SphereCollider3D);
	m_type = ColliderType::SPHERE;
}

/*
void SphereCollider3D::checkCollisionSphere(RigidBody3& rb, const SphereCollider3D& sphere, const float deltaTime)
{
	Vector3f pos = sphere.m_position + sphere.m_transform->getWorldPosition();
	Vector3f back_point = pos - rb.m_velocity.normalized() * (sphere.m_radius + m_radius);
	Vector3f next_pos   = pos + rb.m_velocity * deltaTime;
	Hit out;

	if (intersection_segment_sphere(back_point, next_pos, Sphere(m_position, m_radius + sphere.m_radius), out))
	{
		if (rb.m_velocity.y < -0.5f)
		{
			rb.m_velocity = Vector3f::zero();
			rb.m_next_pos = out.point;
		}
		rb.m_next_pos.y = out.point.y;
	}
}

void SphereCollider3D::checkSegmentIntersection(const Vector3f& A, const Vector3f& B, Vector3f& I, Vector3f& N)
{
	Hit out;
	intersection_segment_sphere(A, B, Sphere(m_transform->m_position, m_radius), out);
	I = out.point;
	N = out.normal;
}
*/

void SphereCollider3D::saveComponentInSCNFile(std::ofstream& file)
{
	file << "SCOLLIDER3\t";

	FileWriter::writeVec3InFile(file, collider.m_center);
	file << "/";
	file << collider.m_radius;
	file << "/";
	file << m_isTrigger;
	file << "/";
	file << m_physicsMat.m_friction;
	file << "/";
	file << m_physicsMat.m_bounciness;
	file << "\n";
}

void SphereCollider3D::loadComponentFromSCNFile(std::istringstream& lineStream)
{
	//	Write in log
	Core::Log* _log = Core::Log::instance();
	_log->write("+\t Adding sphere collider 3D to new gameObject");

	collider.m_center = FileParser::getVector3(lineStream);

	lineStream.ignore();
	collider.m_radius = FileParser::getFloat(lineStream);
	lineStream.ignore();
	m_isTrigger = FileParser::getBool(lineStream);
	lineStream.ignore();
	m_physicsMat.m_friction = FileParser::getFloat(lineStream);
	lineStream.ignore();
	m_physicsMat.m_bounciness = FileParser::getFloat(lineStream);
}

void SphereCollider3D::showImGUI()
{
	using namespace ImGui;
	Checkbox("Is trigger", &m_isTrigger);
	DragFloat3(" <- Collider Position", &collider.m_center.c[0]);
	DragFloat(" <- Collider Radius", &collider.m_radius);
	DragFloat(" <- Friction", &m_physicsMat.m_friction);
	DragFloat(" <- Bounciness", &m_physicsMat.m_bounciness);
}


BoxCollider3D::BoxCollider3D(GameObject* in_gameObject) : Collider3(in_gameObject)
{
	init(ComponentType::BoxCollider3D);
	m_type = ColliderType::BOX;
}

BoxCollider3D::~BoxCollider3D()
{
	Resources::Scene* scene = Core::Graph::instance()->getCurrentScene();
	if (scene)
		scene->m_physicsManager.unregisterCollider(this);
}

/*
void BoxCollider3D::checkCollisionSphere(RigidBody3& rb, const SphereCollider3D& sphere, const float deltaTime)
{
	setAccurateScale();

	Vector3f pos = sphere.m_position + sphere.m_transform->getWorldPosition();

	Vector3f back_point = pos - rb.m_velocity.normalized() * sphere.m_radius;
	Vector3f next_pos   = pos + rb.m_velocity * deltaTime;
	Hit out;

	RoundedBox collisionBox(m_position + m_transform->getWorldPosition(), m_transform->getQuaternionRotation(), m_accurateScale + sphere.m_radius, sphere.m_radius);

	Player* player = nullptr;
	if (rb.m_gameObject->tryGetComponent<Player>(&player)) {}

	if (intersection_segment_roundedbox(back_point, next_pos, collisionBox, out))
	{
		Vector3f collAxisY = out.normal.normalized();
		Vector3f collAxisZ = (vector3CrossProduct(collAxisY, rb.m_velocity)).normalized();;
		Vector3f collAxisX = (vector3CrossProduct(collAxisY, collAxisZ)).normalized();

		Vector3f supportReaction = collAxisY * fabsf(dotProduct(rb.m_velocity, collAxisY));
		Vector3f tangeantReaction = collAxisX * fabsf(dotProduct(rb.m_velocity, collAxisX)) * 0.2f;

		rb.m_velocity += supportReaction + tangeantReaction;

		if (player != nullptr)
		{
			player->m_isColliding = true;

			if (!player->m_isGrounded)
				rb.m_transform->m_position = out.point;
		}
	}
}

void BoxCollider3D::checkSegmentIntersection(const Vector3f& A, const Vector3f& B, Vector3f& I, Vector3f& N)
{
	Hit out;
	intersection_segment_box(A, B, Box(m_position, m_transform->getQuaternionRotation(), m_accurateScale), out);
	I = out.point;
	N = out.normal;
}


bool BoxCollider3D::checkTriggerSphere(RigidBody3& rb, const float radius, const float deltaTime)
{
	Referential3 boxRef = Referential3();
	boxRef.o = m_position;
	boxRef.m_angle = quaternionFromEuler(m_transform->m_rotation);
	Vector3f localSpherePos = globalToLocalPoint3(boxRef, rb.m_transform->m_position);

	if (fabs(localSpherePos.x) > m_accurateScale.x + radius
		|| fabs(localSpherePos.y) > m_accurateScale.y + radius
		|| fabs(localSpherePos.z) > m_accurateScale.z + radius)
		return false;

	Vector3f clampedSpherePos = Vector3f(clamp(localSpherePos.x, -m_accurateScale.x, m_accurateScale.x),
		clamp(localSpherePos.y, -m_accurateScale.y, m_accurateScale.y),
		clamp(localSpherePos.z, -m_accurateScale.z, m_accurateScale.z));

	if (fabs((localSpherePos - clampedSpherePos).squareLength()) > radius * radius)
		return false;

	return true;
}
*/

void BoxCollider3D::awake()
{
	Resources::Scene* scene = Core::Graph::instance()->getNextScene();
	if (scene)
		scene->m_physicsManager.registerCollider(this);
}

void BoxCollider3D::saveComponentInSCNFile(std::ofstream& file)
{
	file << "BCOLLIDER3\t";

	FileWriter::writeVec3InFile(file, collider.m_center);
	file << "/";
	FileWriter::writeVec3InFile(file, collider.m_extension);
	file << "/";
	file << m_isTrigger;
	file << "/";
	file << m_physicsMat.m_friction;
	file << "/";
	file << m_physicsMat.m_bounciness;
	file << "\n";
}

void BoxCollider3D::loadComponentFromSCNFile(std::istringstream& lineStream)
{
	//	Write in log
	Core::Log* _log = Core::Log::instance();
	_log->write("+\t Adding box collider 3D to new gameObject");

	//	Set Rotation Coordinate
	//	-----------------------
	collider.m_center = FileParser::getVector3(lineStream);

	//	Set Rotation Scale
	//	------------------
	lineStream.ignore();
	collider.m_extension = FileParser::getVector3(lineStream);

	lineStream.ignore();
	m_isTrigger = FileParser::getInt(lineStream);

	lineStream.ignore();
	m_physicsMat.m_friction = FileParser::getFloat(lineStream);
	lineStream.ignore();
	m_physicsMat.m_bounciness = FileParser::getFloat(lineStream);
}



void BoxCollider3D::showImGUI()
{
	using namespace ImGui;
	Checkbox("Is trigger", &m_isTrigger);
	DragFloat3(" <- Collider Position", &collider.m_center.c[0]);
	DragFloat3(" <- Collider Scale", &collider.m_extension.c[0]);
	DragFloat(" <- Friction", &m_physicsMat.m_friction);
	DragFloat(" <- Bounciness", &m_physicsMat.m_bounciness);
}


/*
void RoundedBoxCollider3D::checkCollisionSphere(RigidBody3& rb, const float radius, const float deltaTime)
{
	Vector3f back_point = rb.m_transform->m_position - rb.m_velocity.normalized() *( radius + m_radius);
	Vector3f next_pos   = rb.m_transform->m_position + rb.m_velocity * deltaTime;

	float speed = rb.m_velocity.length();
	Hit out;

	RoundedBox BoxCollision(m_colliderPosition, m_transform->getQuaternionRotation(), m_transform->m_scale + radius, m_radius + radius);

	if (intersection_segment_roundedbox(back_point, next_pos, BoxCollision, out))
	{

	}
}

void RoundedBoxCollider3D::checkSegmentIntersection(const Vector3f& A, const Vector3f& B, Vector3f& I, Vector3f& N)
{
	Hit out;
	intersection_segment_roundedbox(A, B, RoundedBox(m_colliderPosition, m_transform->getQuaternionRotation(), m_transform->m_scale, m_radius), out);
	I = out.point;
	N = out.normal;
}


CapsuleCollider3D::CapsuleCollider3D(const Transform3* in_transform, const float in_height, const float in_radius)
{
	m_height = in_height;
	m_radius = in_radius;
}

void CapsuleCollider3D::checkCollisionSphere(RigidBody3& rb, const float radius, const float deltaTime)
{
	Vector3f back_point = rb.m_transform->m_position - rb.m_velocity.normalized() * radius;
	Vector3f next_pos   = rb.m_transform->m_position + rb.m_velocity * deltaTime;
	float speed = rb.m_velocity.length();
	Hit out;

	if (intersection_segment_capsule(back_point, next_pos, Capsule(m_transform.m_position, m_colliderTransform.getQuaternionRotation(), m_height, radius), out))
	{
		//	Get  MovingCollider center to intersection point
		Vector3f AI = (out.point - rb.m_transform->m_position);

		//	Distance between Intersection point and the old next pos
		float distanceIB = getDistance(next_pos, out.point);

		//	New direction
		Vector3f new_dir = vector3Reflect(AI, out.normal).normalized();

		//	New position ( old one, because we need to recheck if there are near collision, before updating new pos)
		rb.m_next_pos = rb.m_transform->m_position;

		//	New Velocity (without force loss for now)
		rb.m_velocity = new_dir * rb.m_velocity.length();
	}
}

void CapsuleCollider3D::checkSegmentIntersection(const Vector3f& A, const Vector3f& B, Vector3f& I, Vector3f& N)
{
	Hit out;
	intersection_segment_capsule(A, B, Capsule(m_transform.m_position, m_colliderTransform.getQuaternionRotation(), m_height, m_colliderScale.x), out);
	I = out.point;
	N = out.normal;
}
*/