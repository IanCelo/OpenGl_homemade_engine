#pragma once
#include "Vector3.h"
#include "Quaternion.h"

namespace  Maths
{
	//	Structures used for intersection detections
	//	-------------------------------------------

	struct Quad
	{
		//	Constructor
		//	-----------
		Quad();
		Quad(const Vector3f& in_center, const Vector2f& in_extension);
		Quad(const Vector3f& in_center, const Quaternion& in_rotation, const Vector2f& in_extension);
		Quad(const Vector3f& in_center, const Vector3f& euler, const Vector2f& in_extension);

		//	Variables
		//	---------
		Vector3f	m_center;
		Vector2f	m_extension;
		Quaternion  m_rotation;
	};

	//	-------------

	struct Disk
	{
		//	Constructor
		//	-----------
		Disk();
		Disk(const Vector3f& normal, const Vector3f& in_center, const float in_radius);

		//	Variables
		//	---------
		Vector3f	m_center = Vector3f::zero();
		float		m_radius = 0.f;
		Quaternion  m_rotation = quaternionIdentity();
	};

	//	-------------

	struct Plane
	{
		//	Constructor
		//	-----------
		Plane();
		Plane(const Vector3f& normal, float d);
		Plane(const Vector3f& normal, const Vector3f& A);
		Plane(const Vector3f& ptA, const Vector3f& ptB, const Vector3f& ptC);
		Plane(Quad in_quad); 
		Plane(Disk in_disk);
		
		//	Variables
		//	---------
		Vector3f	m_normal;
		float		m_distance;
	};

	//	-------------

	struct Sphere
	{
		//	Constructor
		//	-----------
		Sphere();
		Sphere(const Vector3f& center,const float radius);

		//	Variables
		//	---------
		Vector3f	m_center;
		float		m_radius;
		Quaternion  m_rotation;
	};

	//	-------------

	struct Cylinder
	{
		//	Constructor
		//	-----------
		Cylinder();
		Cylinder(const Vector3f& ptA, const Vector3f& ptB, const float radius);
		Cylinder(const Vector3f& center, const Quaternion& in_rotation, const float height,const float radius);

		//	Variables
		//	---------
		Vector3f	m_center;
		float		m_height;
		float		m_radius;
		Quaternion  m_rotation;
	};

	//	-------------

	struct Capsule
	{
		//	Constructor
		//	-----------
		Capsule();
		Capsule(const Vector3f& ptA, const Vector3f& ptB, const float radius);
		Capsule(const Vector3f& center, const float height, const float radius);
		Capsule(const Vector3f& center, const Quaternion& in_rotation, const float height, const float radius);


		//	Variables
		//	---------
		Vector3f	m_center;
		float		m_height;
		float		m_radius;
		Quaternion  m_rotation;
	};

	//	-------------

	struct Box
	{
		//	Constructor
		//	-----------
		Box();
		Box(const Vector3f& in_center, const Quaternion& in_rotation, const Vector3f& in_extension, const float& in_roundness);

		//	Variables
		//	---------
		Vector3f	m_center;
		Vector3f	m_extension;
		Quaternion  m_rotation;
		float m_roundness;
	};

	/*
	//	-------------

	struct RoundedBox
	{
		//	Constructor
		//	-----------
		RoundedBox();
		RoundedBox(Box in_box, float in_radius);
		RoundedBox(const Vector3f& in_center, const Quaternion& in_rotation, const Vector3f& in_extension, float in_radius);

		//	Variables
		//	---------
		Vector3f	m_center;
		Vector3f	m_extension;
		float		m_radius;
		Quaternion  m_rotation;
	};
	*/
}