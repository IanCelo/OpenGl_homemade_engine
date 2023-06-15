#include <Physics/OctreeNode.hpp>
#include <Physics/Collision.hpp>
#include <Physics/Collider3.hpp>
using namespace Physics;

#include <Maths/Quaternion.h>


OctreeNode::OctreeNode(Vector3f center, float halfSize)
{
	AABB = Box(center, quaternionIdentity(), { halfSize, halfSize, halfSize }, 0.0f);
}

void OctreeNode::generate(int depth)
{
	if (depth <= 0)
		return;

	float childrenHalfSize = AABB.m_extension.x * 0.5f;

	for (int i = -1; i < 2; i += 2)
	{
		for (int j = -1; j < 2; j += 2)
		{
			for (int k = -1; k < 2; k += 2)
			{
				// Add child
				Vector3f position = Vector3f{ (float)i, (float)j, (float)k } *childrenHalfSize;
				m_children.push_back(OctreeNode(position, childrenHalfSize));

				// Give it the boxes that he owns
				// and get back the leftovers (by reference)
				m_children.back().scanColliders(m_colliders);

				// If it has no colliders registered, we simply delete it
				if (m_children.back().m_colliders.empty())
					m_children.pop_back();
				else // we iterate the algorithm on it
					m_children.back().generate(depth - 1);
			}
		}
	}
}

void OctreeNode::scanColliders(std::vector<Collider3*>& colliders)
{
	int i = 0;
	while (i < colliders.size())
	{
		switch (colliders[i]->m_type)
		{
		case ColliderType::BOX:
			{
				//BoxCollider3D* boxCol = (BoxCollider3D*)colliders[i];
				//Box box = boxCol->collider;
				//box.m_center += boxCol->m_transform->m_position;
				//if (is_box_inside_AABB(box, AABB))
				//{
				//	m_colliders.push_back(colliders[i]);
				//
				//	colliders[i] = colliders.back();
				//	colliders.pop_back();
				//	this->scanColliders(colliders);
				//	return;
				//}
			}
			break;
		case ColliderType::SPHERE:
			//SphereCollider3D* sphereCol = (SphereCollider3D*)colliders[i];

			break;
		default:
			break;
		}

		++i;
	}

}

void OctreeNode::getPotentialColliders(Collider3* target, std::vector<Collider3*>& potentialColliders)
{
	//if (!is_sphere_intersecting_AABB(AABB, sphere))
	//	return;
	Sphere s = ((SphereCollider3D*)target)->collider;

	if (!is_sphere_inside_AABB(s, AABB))
		return;

	for (int i = 0; i < m_colliders.size(); ++i)
	{
		potentialColliders.push_back(m_colliders[i]);
	}

	for (int i = 0; i < m_children.size(); ++i)
	{
		m_children[i].getPotentialColliders(target, potentialColliders);
	}
}