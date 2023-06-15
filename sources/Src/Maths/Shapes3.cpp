#include "Maths/Shapes3.h"

using namespace Maths;


//	----------------------- QUAD -----------------------

Quad::Quad()
{
	m_center = Vector3f::zero();
	m_rotation = quaternionIdentity();
	m_extension = .5f;
}

Quad::Quad(const Vector3f& in_center, const Vector2f& in_extension)
{
	m_center = in_center;
	m_rotation = quaternionIdentity();
	m_extension = in_extension;
}

Quad::Quad(const Vector3f& in_center, const Quaternion& in_rotation, const Vector2f& in_extension)
{
	m_center = in_center;
	m_rotation = in_rotation;
	m_extension = in_extension;
}

Quad::Quad(const Vector3f& in_center, const Vector3f& euler, const Vector2f& in_extension)
{
	m_center = in_center;
	m_rotation = quaternionFromEuler(euler);
	m_extension = in_extension;
}

//	----------------------- PLANE -----------------------

Plane::Plane()
{
	m_normal = { 0.f,1.f,0.f };
	m_distance = 0.f;
}

Plane::Plane(const Vector3f& normal, float d)
{
	m_normal = normal.normalized();
	m_distance = d;
}

Plane::Plane(const Vector3f& normal, const Vector3f& A)
{
	m_normal = normal.normalized();

	m_distance = dotProduct(A, m_normal) / normal.length();
}

Plane::Plane(const Vector3f& ptA, const Vector3f& ptB, const Vector3f& ptC)
{
	m_normal = vector3CrossProduct(ptB - ptA, ptC - ptA).normalized();
	m_distance = dotProduct(ptA, m_normal);
}

Plane::Plane(Quad in_quad)
{
	m_normal = vector3RotateByQuaternion({ 0.f, 1.f, 0.f }, in_quad.m_rotation);
	m_distance = dotProduct(in_quad.m_center, m_normal) / m_normal.length();
}

Plane::Plane(Disk in_disk)
{
	Vector3f normal = vector3RotateByQuaternion({ 0.f, 1.f, 0.f }, in_disk.m_rotation);
	m_distance = dotProduct(in_disk.m_center, normal) / normal.length();
}


//	----------------------- DISK -----------------------

Disk::Disk()
{
	m_center = Vector3f::zero();
	m_radius = 0.f;
	m_rotation = quaternionIdentity();
}

Disk::Disk(const Vector3f& normal, const Vector3f& pt, float rad)
{
	m_rotation = quaternionFromVector3ToVector3({ 0.f,1.f,0.f }, normal);
	m_center = pt;
	m_radius = rad;
}


//	----------------------- SPHERE -----------------------

Sphere::Sphere()
{
	m_center = Vector3f::zero();
	m_radius = .5f;
	m_rotation = quaternionIdentity();
}


Sphere::Sphere(const Vector3f& center, float radius)
{
	m_center = center;
	m_radius = radius;

	m_rotation = quaternionIdentity();
}


//	----------------------- CYLINDER -----------------------


Cylinder::Cylinder()
{
	m_height = 1.f;
	m_center = Vector3f::zero();;
	m_radius = .5f;
	m_rotation = quaternionIdentity();
}


Cylinder::Cylinder(const Vector3f& ptA, const Vector3f& ptB,const float radius)
{
	m_height = (ptB - ptA).length();
	m_radius = radius;
	m_rotation = quaternionFromVector3ToVector3({ 0.f,1.f,0.f }, (ptB - ptA).normalized());
	m_center = ptA + vector3RotateByQuaternion({ 0.f,m_height * .5f,0.f }, m_rotation);
}


Cylinder::Cylinder(const Vector3f& center, const Quaternion& in_rotation, const float height, const float radius)
{
	m_height = height;
	m_center = center;
	m_radius = radius;
	m_rotation = in_rotation;
}



//	----------------------- CAPSULE -----------------------

Capsule::Capsule()
{
	m_height = 1.f;
	m_center = Vector3f::zero();
	m_radius = .5f;
}

Capsule::Capsule(const Vector3f& ptA, const Vector3f& ptB, float radius)
{
	m_height = (ptB - ptA).length();
	m_center = ptA + m_height * .5f;
	m_radius = radius;
}

Capsule::Capsule(const Vector3f& center, const float height, float radius)
{
	m_height = height;
	m_center = center;
	m_radius = radius;
	m_rotation = quaternionIdentity();
}


Capsule::Capsule(const Vector3f& center, const Quaternion& in_rotation, const float height,const float radius)
{
	m_height = height;
	m_center = center;
	m_radius = radius;
	m_rotation = in_rotation;
}

//	----------------------- BOX -----------------------

Box::Box()
{
	m_center = Vector3f::zero();
	m_extension = .5f;
	m_rotation = quaternionIdentity();
	m_roundness = 0.f;
}

Box::Box(const Vector3f& in_center, const Quaternion& in_rotation, const Vector3f& in_extension, const float& in_roundness)
{
	m_center = in_center;
	m_extension = in_extension;
	m_rotation = in_rotation;
	m_roundness = in_roundness;
}

/*
//	----------------------- ROUNDED BOX -----------------------

RoundedBox::RoundedBox()
{
	m_center = Vector3f::zero();
	m_extension = .5f;
	m_radius = .1f;
	m_rotation = quaternionIdentity();
}

RoundedBox::RoundedBox(Box in_box, float in_radius)
{
	m_center = in_box.m_center;
	m_extension = in_box.m_extension;
	m_radius = in_radius;
	m_rotation = quaternionIdentity();
}

RoundedBox::RoundedBox(const Vector3f& in_center, const Quaternion& in_rotation, const Vector3f& in_extension, float in_radius)
{
	m_center	= in_center;
	m_rotation	= in_rotation;
	m_extension = in_extension;

	m_radius	= in_radius;
}
*/