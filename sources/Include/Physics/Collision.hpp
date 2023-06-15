#pragma once

#include <Physics/Collider3.hpp>
#include <Maths/Intersection3.h>

namespace Physics
{
	// Physics Manager
	bool collision_boxes(const Box& box1, const Box& box2, Hit& hitInfo);
	bool collision_spheres(const Sphere& sphere1, const Vector3f& currPos, const Vector3f& nextPos, const Sphere& sphere2, Hit& hitInfo);
	bool collision_sphere_box(const Sphere& sphere, const Vector3f& currPos, const Vector3f& nextPos, const Box& box, Hit& hitInfo);

	// Octree purposes
	bool is_point_inside_AABB(const Vector3f& point, const Box& box);
	bool is_point_inside_box(const Vector3f& point, const Box& box);
	bool is_box_inside_AABB(const Box& box, const Box& AABB);
	bool is_sphere_inside_AABB(const Sphere& sphere, const Box& AABB);
	bool is_sphere_inside_box(const Sphere& sphere, const Box& box);
	bool is_sphere_inside_sphere(const Sphere& sphere1, const Sphere& sphere2);
}