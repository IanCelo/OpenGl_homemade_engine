#pragma once

#include <Utils/Singleton.h>
#include <unordered_map>

#include <Physics/Collider3.hpp>
#include <Physics/Rigidbody3.hpp>
#include <Physics/OctreeNode.hpp>

namespace Physics
{
	struct RaycastHit
	{
		Hit hit = {};
		Collider3* collider = nullptr;
	};

	class PhysicsManager
	{
	public:
		OctreeNode m_octree;
		std::vector<Collider3*> m_collidersStatic;
		std::vector<Collider3*> m_collidersDynamic;

		std::vector<Rigidbody3*> m_rigidbodies;

		bool m_collisionLayerMatrix[64] = { true };

		void initialize();
		void setUp();

		void registerCollider(Collider3* collider);
		void unregisterCollider(Collider3* collider);

		void registerRigidbody(Rigidbody3* rb);
		void unregisterRigidbody(Rigidbody3* rb);


		void processPhysics();

		bool raycast(Vector3f origin, Vector3f direction, float distance, RaycastHit& hit);
	};
}