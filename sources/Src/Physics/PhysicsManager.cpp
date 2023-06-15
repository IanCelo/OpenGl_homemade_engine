#include <Physics/RigidBody3.hpp>
#include <Physics/Collider3.hpp>

#include <Engine/GameObject.hpp>

#include <Physics/PhysicsManager.hpp>
#include <Physics/Collision.hpp>
using namespace Physics;
#include <Core/TimeManager.h>

void PhysicsManager::initialize()
{
	m_collidersStatic.clear();
	m_collidersDynamic.clear();
	m_rigidbodies.clear();
}

void PhysicsManager::setUp()
{
	m_octree = OctreeNode({ 0, 0, 0 }, 100.f);
	for (auto& collider : m_collidersStatic)
	{
		m_octree.m_colliders.push_back(collider);
	}
	m_octree.generate(5);
}

void PhysicsManager::registerCollider(Collider3* collider)
{
	//if (collider->is_static)
	// m_collidersStatic.push_back(collider);
	//else
	m_collidersDynamic.push_back(collider);
}

void PhysicsManager::registerRigidbody(Rigidbody3* rb)
{
	m_rigidbodies.push_back(rb);
}

void PhysicsManager::unregisterCollider(Collider3* collider)
{
	for (int i = 0; i < m_collidersDynamic.size(); ++i)
	{
		if (m_collidersDynamic[i] == collider)
		{
			m_collidersDynamic[i] = m_collidersDynamic.back();
			m_collidersDynamic.pop_back();
			return;
		}
	}
}

void PhysicsManager::unregisterRigidbody(Rigidbody3* rb)
{
	for (int i = 0; i < m_rigidbodies.size(); ++i)
	{
		if (m_rigidbodies[i] == rb)
		{
			m_rigidbodies[i] = m_rigidbodies.back();
			m_rigidbodies.pop_back();
			return;
		}
	}
}



void PhysicsManager::processPhysics()
{
	// Preventif : remove nullptr in vectors
	m_rigidbodies.erase(std::remove(m_rigidbodies.begin(), m_rigidbodies.end(), nullptr), m_rigidbodies.end());
	m_collidersDynamic.erase(std::remove(m_collidersDynamic.begin(), m_collidersDynamic.end(), nullptr), m_collidersDynamic.end());

	for (Rigidbody3* rb : m_rigidbodies)
	{
		// Compute rigidbody infos
		Vector3f origin = rb->m_transform->m_position;
		Vector3f destination = rb->calculateNextPos();

		if ((SphereCollider3D*)(rb->m_collider) == nullptr) continue;

		Sphere sphere = ((SphereCollider3D*)(rb->m_collider))->collider;
		sphere.m_center += origin;


		// Compute static colliders
		std::vector<Collider3*> potentialStaticColliders;
		m_octree.getPotentialColliders(rb->m_collider, potentialStaticColliders);


		// Create all-colliders list
		std::vector<Collider3*> collidersToCheck;
		for (auto& col : m_collidersDynamic)
			collidersToCheck.push_back(col);
		for (auto& col : potentialStaticColliders)
			collidersToCheck.push_back(col);

		for (Rigidbody3* other : m_rigidbodies)
		{
			if (other != rb)
			{
				collidersToCheck.push_back(other->m_collider);
			}
		}


		// Loop on each collider
		int i = 0;
		while (i < collidersToCheck.size())
		{
			Collider3* collider = collidersToCheck[i];

			Hit hitInfo;
			bool collision = false;

			if (collider == nullptr || collider->m_isTrigger || !collider->isActive())
			{
				i++;
				continue;
			}

			if (collider->m_type == ColliderType::BOX)
			{
				BoxCollider3D* col = (BoxCollider3D*)collider;
				if (col == nullptr)
				{
					i++;
					continue;
				}

				Box box = col->collider;
				box.m_center += col->m_gameObject->m_transform->m_position;
				box.m_rotation = quaternionFromEuler(col->m_transform->m_rotation);
				box.m_extension = { col->collider.m_extension.x * col->m_transform->m_scale.x,
									col->collider.m_extension.y * col->m_transform->m_scale.y,
									col->collider.m_extension.z * col->m_transform->m_scale.z };

				collision = collision_sphere_box(sphere, origin, destination, box, hitInfo);
			}
			else if (collider->m_type == ColliderType::SPHERE)
			{
				SphereCollider3D* col = (SphereCollider3D*)collider;
				if (col == nullptr)
				{
					i++;
					continue;
				}

				Sphere sph = col->collider;
				sph.m_center += col->m_gameObject->m_transform->m_position;
				sph.m_rotation = quaternionFromEuler(col->m_transform->m_rotation);
				sph.m_radius *= max(max(col->m_transform->m_scale.x, col->m_transform->m_scale.y), col->m_transform->m_scale.z);

				collision = collision_spheres(sphere, origin, destination, sph, hitInfo);
			}

			// If collision, then reaction of the collider on the rigidbody
			if (collision)
			{
				Vector3f axisY = hitInfo.normal.normalized();

				// Compute collision reaction
				Vector3f posToPoint = hitInfo.point - rb->m_transform->m_position;
				float signedDistanceToPoint = dotProduct(posToPoint, rb->m_velocity.normalized());
				float multiplier = signedDistanceToPoint < 0 ? 2 : 1.0001f;

				// Calculate velocity projected on the referential
				float pY = dotProduct(rb->m_velocity, axisY);

				//Vector3f reactionForce = axisY * fabsf(pY) * multiplier;
				Vector3f reactionForce = axisY * fabsf(pY) * multiplier;
				Vector3f tangeantForce = -(rb->m_velocity + reactionForce) * collider->m_physicsMat.m_friction;

				rb->m_velocity += reactionForce + tangeantForce;

				// Reset everything so we loop back on all the other ones
				destination = rb->m_transform->m_position + rb->m_velocity * Core::TimeManager::instance()->fixedDeltaTime;
				collidersToCheck[i] = collidersToCheck.back();
				collidersToCheck.pop_back();
				i = 0;
			}
			else
			{
				i++;
			}
		}

		rb->updatePosition();
	}
}

bool PhysicsManager::raycast(Vector3f origin, Vector3f direction, float distance, RaycastHit& hit)
{
	bool collided = false;

	for (auto& collider : m_collidersDynamic)
	{
		BoxCollider3D* col = (BoxCollider3D*)collider;

		if (col == nullptr || !collider->m_gameObject->isActive())
			continue;

		Box box = col->collider;
		box.m_center = col->m_gameObject->m_transform->m_position + col->collider.m_center;
		box.m_rotation = quaternionFromEuler(col->m_transform->m_rotation);
		box.m_extension = { col->collider.m_extension.x * col->m_transform->m_scale.x,
							col->collider.m_extension.y * col->m_transform->m_scale.y,
							col->collider.m_extension.z * col->m_transform->m_scale.z };

		Hit hitInfo;

		if (intersection_segment_box(origin, origin + direction * distance, box, hitInfo))
		{
			if (collided)
			{
				if ((hitInfo.point - origin).squareLength() < (hit.hit.point - origin).squareLength())
					hit = { hitInfo, col };
			}
			else
				hit = { hitInfo, col };

			collided = true;
		}
	}

	if (collided)
		return true;

	return false;
}
