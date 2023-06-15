#pragma once

#include <vector>

#include <Physics/Collider3.hpp>

namespace Physics
{
	class OctreeNode
	{
	private:
		Box AABB;

	public:
		std::vector<OctreeNode> m_children;
		std::vector<Collider3*> m_colliders;

		OctreeNode() = default;
		OctreeNode(Vector3f center, float halfSize);

		void generate(int depth);

		void scanColliders(std::vector<Collider3*>& colliders);

		void getPotentialColliders(Collider3* target, std::vector<Collider3*>& potentialColliders);
	};
}