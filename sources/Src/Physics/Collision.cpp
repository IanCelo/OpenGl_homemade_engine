#include <Physics/Collision.hpp>


bool Physics::collision_boxes(const Box& box1, const Box& box2, Hit& hitInfo)
{

	return false;
}
bool Physics::collision_spheres(const Sphere& sphere1, const Vector3f& currPos, const Vector3f& nextPos, const Sphere& sphere2, Hit& hitInfo)
{
	Vector3f ab = nextPos - currPos;
	Vector3f abLonger = ab.normalized() * (sphere1.m_radius + sphere1.m_radius);
	Vector3f pos = currPos - abLonger;

	Sphere sphere = Sphere(sphere2.m_center, sphere1.m_radius + sphere2.m_radius);

	return intersection_segment_sphere(pos, nextPos, sphere, hitInfo);
}
bool Physics::collision_sphere_box(const Sphere& sphere, const Vector3f& currPos, const Vector3f& nextPos, const Box& box, Hit& hitInfo)
{
	Box rBox = Box(box.m_center, box.m_rotation, box.m_extension + sphere.m_radius, box.m_roundness + sphere.m_radius);

	return intersection_segment_box(currPos, nextPos, rBox, hitInfo);
}


bool is_value_in_range(float value, float min, float max)
{
	return value >= min && value <= max;
}

bool Physics::is_point_inside_AABB(const Vector3f& point, const Box& box)
{
	return is_value_in_range(point.x, box.m_center.x - box.m_extension.x, box.m_center.x + box.m_extension.x) &&
		is_value_in_range(point.y, box.m_center.y - box.m_extension.y, box.m_center.y + box.m_extension.y) &&
		is_value_in_range(point.z, box.m_center.z - box.m_extension.z, box.m_center.z + box.m_extension.z);
}

bool Physics::is_point_inside_box(const Vector3f& point, const Box& box)
{
	return false;
}

bool Physics::is_box_inside_AABB(const Box& box, const Box& AABB)
{
	for (int i = -1; i <= 1; i += 2)
	{
		for (int j = -1; j <= 1; j += 2)
		{
			for (int k = -1; k <= 1; k += 2)
			{
				Vector3f corner = { i * (box.m_extension.x + box.m_roundness), j * (box.m_extension.y + box.m_roundness), k * (box.m_extension.z + box.m_roundness) };
				if (!is_point_inside_AABB(vector3RotateByQuaternion(corner, box.m_rotation) + box.m_center, AABB))
					return false;
			}
		}
	}

	for (int i = -1; i <= 1; i += 2)
	{
		for (int j = -1; j <= 1; j += 2)
		{
			for (int k = -1; k <= 1; k += 2)
			{
				Vector3f corner = { i * (AABB.m_extension.x + AABB.m_roundness), j * (AABB.m_extension.y + AABB.m_roundness), k * (AABB.m_extension.z + AABB.m_roundness) };
				if (is_point_inside_AABB(corner + AABB.m_center, box))
					return false;
			}
		}
	}

	return true;
}
bool Physics::is_sphere_inside_AABB(const Sphere& sphere, const Box& AABB)
{
	Box sphereBox = Box(sphere.m_center, quaternionIdentity(), Vector3f{ sphere.m_radius, sphere.m_radius, sphere.m_radius }, 0.0f);

	for (int i = -1; i <= 1; i += 2)
	{
		for (int j = -1; j <= 1; j += 2)
		{
			for (int k = -1; k <= 1; k += 2)
			{
				Vector3f corner = { i * (sphereBox.m_extension.x + sphereBox.m_roundness), j * (sphereBox.m_extension.y + sphereBox.m_roundness), k * (sphereBox.m_extension.z + sphereBox.m_roundness) };
				if (is_point_inside_box(vector3RotateByQuaternion(corner, sphereBox.m_rotation) + sphereBox.m_center, AABB))
					return true;
			}
		}
	}

	return false;
}

bool Physics::is_sphere_inside_box(const Sphere& sphere, const Box& box)
{
	Vector3f rayon = Vector3f(sphere.m_center, box.m_center).normalized() * sphere.m_radius;
	return is_point_inside_box(sphere.m_center + rayon, box);
}