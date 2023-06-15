#include "Maths/Intersection3.h"
#include "Maths/Utils.h"

#define INTERSECTION	true
#define NO_INTERSECTION	false



bool Maths::intersection_segment_plane(const Vector3f& ptA, const Vector3f& ptB, const Plane& in_plane, Hit& out_hit)
{
	Vector3f vect_AB = ptB - ptA;
	float dot_AB_n = dotProduct(vect_AB, in_plane.m_normal);

	//	If AB and the plane normal are orthogonal, it means that the plane and AB are colinear
	//	So there is no intersections
	if (fabs(dot_AB_n) < EPSILON)
	{
		return NO_INTERSECTION;
	}

	//	Dot product of AP  (P is the nearest plane point from the origin) and the plane normal
	float dot_AP_n = dotProduct(in_plane.m_normal * in_plane.m_distance - ptA, in_plane.m_normal);

	float t = dot_AP_n / dot_AB_n;

	//	Because it is a segment if AP.n and AB.n have different sign, or if AP.n is superior to AB.n,
	//	[AB] can't intersect with the plane
	if (t < 0 || t > 1)
	{
		return NO_INTERSECTION;
	}

	//	t allow us to obtain the position of the intersection point on the segment AB
	out_hit.point = ptA + vect_AB * t;
	out_hit.normal = in_plane.m_normal;

	return INTERSECTION;
}


bool Maths::intersection_line_plane(const Vector3f& ptA, const Vector3f& ptB, const  Plane& in_plane, Hit& out_hit)
{
	Vector3f vect_AB = ptB - ptA;
	float dot_AB_n = dotProduct(vect_AB, in_plane.m_normal);

	//	If AB and the plane normal are orthogonal, it means that the plane and AB are colinear
	//	So there is no intersections
	if (abs(dot_AB_n) < EPSILON)
	{
		return NO_INTERSECTION;
	}

	//	Else, because it is a line and a plane with infinite length, there is an intersection for sure

	float dot_AP_n = dotProduct(in_plane.m_normal * in_plane.m_distance - ptA, in_plane.m_normal);

	float t = dot_AP_n / dot_AB_n;

	//	t allow us to obtain the position of the intersection point on the line AB
	out_hit.point = ptA + vect_AB * t;
	out_hit.normal = in_plane.m_normal;

	return INTERSECTION;
}



bool Maths::intersection_segment_disk(const Vector3f& ptA, const Vector3f& ptB, const Disk& in_disk, Hit& out_hit)
{
	if (!intersection_segment_plane(ptA, ptB, Plane(in_disk), out_hit))
	{
		return NO_INTERSECTION;
	}

	Vector3f _PI = out_hit.point - in_disk.m_center;

	if (_PI.squareLength() <= in_disk.m_radius * in_disk.m_radius)
	{
		return INTERSECTION;
	}

	return NO_INTERSECTION;
}

bool Maths::intersection_segment_sphere(const Vector3f& ptA, const Vector3f& ptB, const Sphere& in_sphere, Hit& out_hit)
{
	if (ptB == ptA) return NO_INTERSECTION;

	//	Here, O is the sphere center

	Vector3f AB = ptB - ptA;
	Vector3f OA = ptA - in_sphere.m_center;

	//	Second degree polynomial to find t
	float a = AB.squareLength();;
	float b = 2.f * dotProduct(OA, AB);
	float c = OA.squareLength() - (in_sphere.m_radius * in_sphere.m_radius);
	float delta = (b * b) - (4 * a * c);

	if (delta < 0.f) return NO_INTERSECTION;


	float t = -(b + sqrtf(delta)) / (2 * a);

	if (t < 0 || t > 1)
	{
		return NO_INTERSECTION;
	}

	//	t allow us to obtain the position of the intersection point on the segment AB
	out_hit.point = ptA + AB * t;
	out_hit.normal = (out_hit.point - in_sphere.m_center).normalized();

	return INTERSECTION;
}

bool Maths::intersection_segment_inf_cylinder(const Vector3f& ptA, const Vector3f& ptB, const Cylinder& in_cyl, Hit& out_hit)
{
	if (in_cyl.m_height == 0.f) return NO_INTERSECTION;

	Vector3f ptP = in_cyl.m_center - vector3RotateByQuaternion({ 0,in_cyl.m_height * .5f,0 }, in_cyl.m_rotation);
	Vector3f ptQ = in_cyl.m_center + vector3RotateByQuaternion({ 0,in_cyl.m_height * .5f,0 }, in_cyl.m_rotation);
	Vector3f AB = ptB - ptA;
	Vector3f PQ = ptQ - ptP;
	Vector3f PA = ptA - ptP;

	float dot_AB_PQ = dotProduct(AB, PQ);
	float dot_PA_PQ = dotProduct(PA, PQ);
	float dot_PA_AB = dotProduct(PA, AB);

	float dot_AB_AB = dotProduct(AB, AB);
	float dot_PQ_PQ = dotProduct(PQ, PQ);
	float dot_PA_PA = dotProduct(PA, PA);

	float a = dot_PQ_PQ * dot_AB_AB - dot_AB_PQ * dot_AB_PQ;
	float b = 2 * (dot_PQ_PQ * dot_PA_AB - dot_AB_PQ * dot_PA_PQ);
	float c = dot_PQ_PQ * (dot_PA_PA - in_cyl.m_radius * in_cyl.m_radius) - dot_PA_PQ * dot_PA_PQ;

	float delta = (b * b) - (4 * a * c);

	if (delta < 0.f) return NO_INTERSECTION;

	float t = -(b + sqrt(delta)) / (2 * a);

	if (t > 1 || t < 0)
	{
		return NO_INTERSECTION;
	}

	//	t allow us to obtain the position of the intersection point on the segment AB
	out_hit.point = ptA + AB * t;


	return INTERSECTION;
}

bool Maths::intersection_segment_cylinder(const Vector3f& ptA, const Vector3f& ptB, const Cylinder& in_cyl, Hit& out_hit)
{

	if (in_cyl.m_height == 0.f) return NO_INTERSECTION;

	Vector3f ptP = in_cyl.m_center - vector3RotateByQuaternion({ 0,in_cyl.m_height * .5f,0 }, in_cyl.m_rotation);
	Vector3f ptQ = in_cyl.m_center + vector3RotateByQuaternion({ 0,in_cyl.m_height * .5f,0 }, in_cyl.m_rotation);

	Vector3f AB = ptB - ptA;
	Vector3f PQ = ptQ - ptP;
	Vector3f PA = ptA - ptP;

	float dot_AB_PQ = dotProduct(AB, PQ);
	float dot_PA_PQ = dotProduct(PA, PQ);
	float dot_PA_AB = dotProduct(PA, AB);

	float dot_AB_AB = dotProduct(AB, AB);
	float dot_PQ_PQ = dotProduct(PQ, PQ);
	float dot_PA_PA = dotProduct(PA, PA);

	float a = dot_PQ_PQ * dot_AB_AB - dot_AB_PQ * dot_AB_PQ;
	float c = dot_PQ_PQ * (dot_PA_PA - in_cyl.m_radius * in_cyl.m_radius) - dot_PA_PQ * dot_PA_PQ;

	if (abs(a) < EPSILON && c > 0.f) return NO_INTERSECTION;

	//	If c < 0, it means that there is no collision on the rounded faces, so let's check the disks on the top/bottom 
	if (c < 0.f)
	{
		if (dot_PA_PQ < 0.f)
		{
			return intersection_segment_disk(ptA, ptB, Disk(-PQ.normalized(), ptP, in_cyl.m_radius), out_hit);
		}

		if (dot_PA_PQ > dot_PQ_PQ)
		{
			return intersection_segment_disk(ptA, ptB, Disk(PQ.normalized(), ptQ, in_cyl.m_radius), out_hit);
		}

		return NO_INTERSECTION;
	}

	float b = (dot_PQ_PQ * dot_PA_AB - dot_AB_PQ * dot_PA_PQ);
	float delta = (b * b) - (a * c);

	if (delta < 0.f) return NO_INTERSECTION;

	if (delta > 0.f)
	{
		float t = -(b + sqrt(delta)) / a;

		if (t > 1 || t < 0)
		{
			return NO_INTERSECTION;
		}

		out_hit.point = ptA + AB * t;
	}

	float dot_PI_PQ = dotProduct(out_hit.point - ptP, PQ);

	if (dot_PI_PQ < 0)
	{
		return intersection_segment_disk(ptA, ptB, Disk(-PQ.normalized(), ptP, in_cyl.m_radius), out_hit);

	}

	if (dot_PI_PQ > dot_PQ_PQ)
	{
		return intersection_segment_disk(ptA, ptB, Disk(PQ.normalized(), ptQ, in_cyl.m_radius), out_hit);
	}
	out_hit.normal = getVector3Perpendicular(out_hit.point - ptP, PQ).normalized();

	return INTERSECTION;
}



bool Maths::intersection_segment_capsule(const Vector3f& ptA, const Vector3f& ptB, const Capsule& in_cap, Hit& out_hit)
{
	Vector3f axis = vector3RotateByQuaternion({ 0, 1, 0 }, in_cap.m_rotation);
	Vector3f diff = axis * in_cap.m_height * 0.5f;

	Vector3f ptP = in_cap.m_center - diff;
	Vector3f ptQ = in_cap.m_center + diff;

	Vector3f AB = ptB - ptA;
	Vector3f PQ = ptQ - ptP;
	Vector3f PA = ptA - ptP;

	//if (PQ == Vector3f::zero())
	//{
	//	return intersection_segment_sphere(ptA, ptB, Sphere(in_cap.m_center, in_cap.m_radius), out_hit);
	//}


	float dot_AB_PQ = dotProduct(AB, PQ);
	float dot_PA_PQ = dotProduct(PA, PQ);
	float dot_PA_AB = dotProduct(PA, AB);

	float dot_AB_AB = dotProduct(AB, AB);
	float dot_PQ_PQ = dotProduct(PQ, PQ);
	float dot_PA_PA = dotProduct(PA, PA);

	float a = dot_PQ_PQ * dot_AB_AB - dot_AB_PQ * dot_AB_PQ;
	float c = dot_PQ_PQ * (dot_PA_PA - in_cap.m_radius * in_cap.m_radius) - dot_PA_PQ * dot_PA_PQ;

	if ((fabsf(a) < EPSILON) && c > 0.f) return NO_INTERSECTION;


	if (c < 0.f)
	{
		if (dot_PA_PQ > 0 && dot_PA_PQ < dot_PQ_PQ) return NO_INTERSECTION;

		if (dot_PA_PQ < 0.f)
		{
			return intersection_segment_sphere(ptA, ptB, Sphere(ptP, in_cap.m_radius), out_hit);
		}

		if (dot_PA_PQ > dot_PQ_PQ)
		{
			return intersection_segment_sphere(ptA, ptB, Sphere(ptQ, in_cap.m_radius), out_hit);
		}

		return NO_INTERSECTION;
	}

	float b = (dot_PQ_PQ * dot_PA_AB - dot_AB_PQ * dot_PA_PQ);
	float delta = (b * b) - (a * c);

	if (delta < 0.f)
	{
		return NO_INTERSECTION;
	}

	float t = (-b - sqrt(delta)) / a;

	if (t > 1 || t < 0)
	{
		return NO_INTERSECTION;
	}

	out_hit.point = ptA + AB * t;

	float dot_PI_PQ = dotProduct(out_hit.point - ptP, PQ);

	if (dot_PI_PQ < 0)
	{
		return intersection_segment_sphere(ptA, ptB, Sphere(ptP, in_cap.m_radius), out_hit);
	}

	if (dot_PI_PQ > dot_PQ_PQ)
	{
		return intersection_segment_sphere(ptA, ptB, Sphere(ptQ, in_cap.m_radius), out_hit);
	}

	Plane plan(PQ.normalized(), out_hit.point);
	Hit hit;
	if (intersection_segment_plane(ptP, ptQ, plan, hit))
	{
		out_hit.normal = (out_hit.point - hit.point).normalized();
	}

	return INTERSECTION;
}


bool Maths::intersection_segment_quad(const Vector3f& ptA, const Vector3f& ptB, const Quad& in_quad, Hit& out_hit)
{
	if (in_quad.m_extension.x == 0.f || in_quad.m_extension.y == 0.f)	 return NO_INTERSECTION;
	if (intersection_segment_plane(ptA, ptB, in_quad, out_hit) == false) return NO_INTERSECTION;

	Vector3f OI = (out_hit.point - in_quad.m_center);
	Vector3f x = vector3RotateByQuaternion(Vector3f(1.f, 0.f, 0.f), in_quad.m_rotation);
	Vector3f y = vector3RotateByQuaternion(Vector3f(0.f, 0.f, 1.f), in_quad.m_rotation);

	if (abs(dotProduct(OI, x)) > in_quad.m_extension.x || abs(dotProduct(OI, y)) > in_quad.m_extension.y) return NO_INTERSECTION;

	return INTERSECTION;
}


/*
bool Maths::intersection_segment_quad(const Vector3f& ptA, const Vector3f& ptB, const Quad& in_quad, Hit& out_hit)
{
	Vector3f normal = vector3RotateByQuaternion({ 0, 1, 0 }, in_quad.m_rotation);

	if (dotProduct(normal, Vector3f(ptA, ptB)) > 0)
		return false;

	Plane plane(normal, in_quad.m_center);

	if (!intersection_segment_plane(ptA, ptB, plane, out_hit))
		return NO_INTERSECTION;

	Vector3f vect = Vector3f(in_quad.m_center, out_hit.point);

	Vector3f i = vector3RotateByQuaternion({ 1, 0, 0 }, in_quad.m_rotation).normalized();
	Vector3f j = vector3RotateByQuaternion({ 0, 0, 1 }, in_quad.m_rotation).normalized();

	float proj1 = dotProduct(vect, i);
	float proj2 = dotProduct(vect, j);

	if (fabsf(proj1) > in_quad.m_extension.x)
		return NO_INTERSECTION;

	if (fabsf(proj2) > in_quad.m_extension.y)
		return NO_INTERSECTION;

	return true;
}
*/

/*
bool intersect_segment_voronol_region(const Vector3f& ptA, const Vector3f& ptB, const Quad& in_quad, const Box& in_box, Hit& out_hit)
{
	if (in_box.m_roundness <= 0.0f)
		return INTERSECTION;

	Quad shortQuad = Quad(in_quad.m_center, in_quad.m_rotation, in_quad.m_extension - in_box.m_roundness);
	if (intersection_segment_quad(ptA, ptB, shortQuad, out_hit))
		return INTERSECTION;

	Vector3f normal = vector3RotateByQuaternion({ 0, 1, 0 }, shortQuad.m_rotation);

	if (dotProduct(normal, ptB - ptA) > 0)
		return NO_INTERSECTION;

	Plane plane(normal, shortQuad.m_center);
	if (!intersection_segment_plane(ptA, ptB, plane, out_hit))
		return NO_INTERSECTION;

	// We first find the quad referential (i, normal, k) with normal being the up (j) vector
	Vector3f i = vector3RotateByQuaternion({ 1, 0, 0 }, shortQuad.m_rotation).normalized();
	Vector3f k = vector3RotateByQuaternion({ 0, 0, 1 }, shortQuad.m_rotation).normalized();

	// We project the intersection point on the quad referential
	Vector3f vect = out_hit.point - shortQuad.m_center;
	float projX = dotProduct(vect, i);
	float projZ = dotProduct(vect, k);

	bool isInsideOnX = fabs(projX) < shortQuad.m_extension.x;
	bool isInsideOnZ = fabs(projZ) < shortQuad.m_extension.y;

	// If we are in the voronoi face -> intersection !
	if (isInsideOnX && isInsideOnZ)
		return INTERSECTION;

	// Then we are not in the voronoi face,
	// So we should be in one of the 4 capsules around the quad

	// We analyze de sign of the projection to get the position of the supposely capsules
	int signX = projX > 0 ? 1 : -1;
	int signZ = projZ > 0 ? 1 : -1;

	// If the interPoint is good in X-axis,
	// we should have only 1 capsule to check
	if (isInsideOnX)
	{
		Quaternion rotation = shortQuad.m_rotation * quaternionFromAxisAngle({ 0, 0, 1 }, (float)PI * 0.5f);
		Vector3f position = shortQuad.m_center - (normal * in_box.m_roundness) + k * shortQuad.m_extension.y * (float)signZ;
		Capsule caps = Capsule(position, rotation, shortQuad.m_extension.x * 2, in_box.m_roundness);

		return (intersection_segment_capsule(ptA, ptB, caps, out_hit));
	}

	// If the interPoint is good in Z-axis,
	// we should have only 1 capsule to check
	if (isInsideOnZ)
	{
		//Quaternion mult = QuaternionMultiply(quad.quaternion, QuaternionFromAxisAngle(normal.toRlVec(), PI * 0.5f));
		Quaternion rotation = shortQuad.m_rotation * quaternionFromAxisAngle({ 1, 0, 0 }, (float)PI * 0.5f);
		Vector3f position = shortQuad.m_center - (normal * in_box.m_roundness) + i * shortQuad.m_extension.x * (float)signX;
		Capsule caps = Capsule(position, rotation, shortQuad.m_extension.y * 2, in_box.m_roundness);

		return (intersection_segment_capsule(ptA, ptB, caps, out_hit));
	}

	// If it is not, we double check with the 2 capsules
	// To get the intersection supposedly being in the capsule sphere
	Quaternion rotationZ = shortQuad.m_rotation * quaternionFromAxisAngle({ 1, 0, 0 }, (float)PI * 0.5f);
	Vector3f positionZ = shortQuad.m_center - (normal * in_box.m_roundness) + i * shortQuad.m_extension.x * (float)signX;
	Capsule capsZ = Capsule(positionZ, rotationZ, shortQuad.m_extension.y * 2, in_box.m_roundness);

	Quaternion rotationX = shortQuad.m_rotation * quaternionFromAxisAngle({ 0, 0, 1 }, (float)PI * 0.5f);
	Vector3f positionX = shortQuad.m_center - (normal * in_box.m_roundness) + k * shortQuad.m_extension.y * (float)signZ;
	Capsule capsX = Capsule(positionX, rotationX, shortQuad.m_extension.x * 2, in_box.m_roundness);

	// If at least one intersect, we got it !
	if (intersection_segment_capsule(ptA, ptB, capsX, out_hit)
		|| intersection_segment_capsule(ptA, ptB, capsZ, out_hit))
		return true;

	return false;

	return NO_INTERSECTION;
}


bool Maths::intersection_segment_box(const Vector3f& ptA, const Vector3f& ptB, const Box& in_box, Hit& out_hit)
{
	Box box = in_box;
	box.m_roundness = clamp(box.m_roundness, 0.f, box.m_extension.x);
	box.m_roundness = clamp(box.m_roundness, 0.f, box.m_extension.y);
	box.m_roundness = clamp(box.m_roundness, 0.f, box.m_extension.z);

	Vector3f i = vector3RotateByQuaternion({ 1.f,0.f,0.f }, in_box.m_rotation);
	Vector3f j = vector3RotateByQuaternion({ 0.f,1.f,0.f }, in_box.m_rotation);
	Vector3f k = vector3RotateByQuaternion({ 0.f,0.f,1.f }, in_box.m_rotation);
	Vector3f AB = Vector3f(ptA, ptB);

	//	Upper face intersection check
	if (dotProduct(j, AB) < 0.f)
	{
		Quad upperFace = Quad(vector3RotateByQuaternion({ 0, box.m_extension.y, 0, }, box.m_rotation) + box.m_center,
			box.m_rotation * quaternionFromAxisAngle({ 1, 0, 0 }, 0.f),
			Vector2f{ box.m_extension.x, box.m_extension.z });

		if (intersection_segment_quad(ptA, ptB, upperFace, out_hit))
		{
			return  intersect_segment_voronol_region(ptA, ptB, upperFace, box, out_hit);
		}
	}
	//	Bottom face intersection check
	else
	{
		Quad bottomFace = Quad(vector3RotateByQuaternion({ 0, -box.m_extension.y, 0, }, box.m_rotation) + box.m_center,
			box.m_rotation * quaternionFromAxisAngle({ 1, 0, 0 }, (float)PI),
			Vector2f{ box.m_extension.x, box.m_extension.z });

		if (intersection_segment_quad(ptA, ptB, bottomFace, out_hit))
		{
			return  intersect_segment_voronol_region(ptA, ptB, bottomFace, box, out_hit);
		}
	}

	//	Right face intersection check
	if (dotProduct(i, AB) < 0.f)
	{
		Quad rightFace = Quad(vector3RotateByQuaternion({ box.m_extension.x, 0, 0, }, box.m_rotation) + box.m_center,
			box.m_rotation * quaternionFromAxisAngle({ 0, 0, 1 }, (float)-PI * 0.5f),
			Vector2f{ box.m_extension.y, box.m_extension.z });

		if (intersection_segment_quad(ptA, ptB, rightFace, out_hit))
		{
			return  intersect_segment_voronol_region(ptA, ptB, rightFace, box, out_hit);
		}
	}
	//	Left face intersection check
	else
	{
		Quad leftFace = Quad(vector3RotateByQuaternion({ -box.m_extension.x, 0, 0, }, box.m_rotation) + box.m_center,
			box.m_rotation * quaternionFromAxisAngle({ 0, 0, 1 }, (float)PI * 0.5f),
			Vector2f{ box.m_extension.y, box.m_extension.z });

		if (intersection_segment_quad(ptA, ptB, leftFace, out_hit))
		{
			return  intersect_segment_voronol_region(ptA, ptB, leftFace, box, out_hit);
		}
	}

	//	Front face intersection check
	if (dotProduct(k, AB) < 0.f)
	{
		Quad frontFace = Quad(vector3RotateByQuaternion({ 0, 0, box.m_extension.z }, box.m_rotation) + box.m_center,
			box.m_rotation * quaternionFromAxisAngle({ 1, 0, 0 }, (float)PI * 0.5f),
			Vector2f{ box.m_extension.x, box.m_extension.y });

		if (intersection_segment_quad(ptA, ptB, frontFace, out_hit))
		{
			return  intersect_segment_voronol_region(ptA, ptB, frontFace, box, out_hit);
		}
	}
	//	Back face intersection check
	else
	{
		Quad backFace = Quad(vector3RotateByQuaternion({ 0, 0, -box.m_extension.z }, box.m_rotation) + box.m_center,
			box.m_rotation * quaternionFromAxisAngle({ 1, 0, 0 }, (float)-PI * 0.5f),
			Vector2f{ box.m_extension.x, box.m_extension.y });

		if (intersection_segment_quad(ptA, ptB, backFace, out_hit))
		{
			return  intersect_segment_voronol_region(ptA, ptB, backFace, box, out_hit);
		}
	}

	return NO_INTERSECTION;
}
*/

/*
void CheckNearestIntersection(Hit& out_hit, const Vector3f& ptA, const Hit& in_hit, bool& intersected)
{
	if (intersected == NO_INTERSECTION)
	{
		out_hit.point = in_hit.point;
		out_hit.normal = in_hit.normal;
	}
	else if ((out_hit.point - ptA).squareLength() < (out_hit.point - ptA).squareLength())
	{
		out_hit.point = in_hit.point;
		out_hit.normal = in_hit.normal;
	}

	intersected = INTERSECTION;
}
*/

/*
bool Maths::intersection_segment_box(const Vector3f& ptA, const Vector3f& ptB, const Box& in_box, Hit& out_hit)
{
	Vector3f i = vector3RotateByQuaternion({ 1.f,0.f,0.f }, in_box.m_rotation);
	Vector3f j = vector3RotateByQuaternion({ 0.f,1.f,0.f }, in_box.m_rotation);
	Vector3f k = vector3RotateByQuaternion({ 0.f,0.f,1.f }, in_box.m_rotation);
	Vector3f AB = Vector3f(ptA, ptB);

	//	Upper face intersection check
	if (dotProduct(j, AB) < 0.f)
	{
		Quad upperFace = Quad(in_box.m_center + j * in_box.m_extension.y, in_box.m_rotation, Vector2f(in_box.m_extension.x, in_box.m_extension.z));

		if (intersection_segment_quad(ptA, ptB, upperFace, out_hit))
		{
			return  INTERSECTION;
		}
	}
	//	Bottom face intersection check
	else
	{
		Quad bottomFace = Quad(in_box.m_center - j * in_box.m_extension.y, in_box.m_rotation * quaternionFromEuler(zVector3((float)PI)), Vector2f(in_box.m_extension.x, in_box.m_extension.z));

		if (intersection_segment_quad(ptA, ptB, bottomFace, out_hit))
		{
			return  INTERSECTION;
		}
	}

	//	Right face intersection check
	if (dotProduct(i, AB) < 0.f)
	{
		Quad rightFace = Quad(in_box.m_center + i * in_box.m_extension.x, in_box.m_rotation * quaternionFromEuler(zVector3(-(float)HALF_PI)), Vector2f(in_box.m_extension.y, in_box.m_extension.z));

		if (intersection_segment_quad(ptA, ptB, rightFace, out_hit))
		{
			return  INTERSECTION;
		}
	}
	//	Left face intersection check
	else
	{
		Quad leftFace = Quad(in_box.m_center - i * in_box.m_extension.x, in_box.m_rotation * quaternionFromEuler(zVector3((float)HALF_PI)), Vector2f(in_box.m_extension.y, in_box.m_extension.z));

		if (intersection_segment_quad(ptA, ptB, leftFace, out_hit))
		{
			return  INTERSECTION;
		}
	}

	//	Front face intersection check
	if (dotProduct(k, AB) < 0.f)
	{

		Quad frontFace = Quad(in_box.m_center + k * in_box.m_extension.z, in_box.m_rotation * quaternionFromEuler(xVector3((float)HALF_PI)), Vector2f(in_box.m_extension.x, in_box.m_extension.y));

		if (intersection_segment_quad(ptA, ptB, frontFace, out_hit))
		{
			return  INTERSECTION;
		}
	}
	//	Back face intersection check
	else
	{
		Quad backFace = Quad(in_box.m_center - k * in_box.m_extension.z, in_box.m_rotation * quaternionFromEuler(xVector3(-(float)HALF_PI)), Vector2f(in_box.m_extension.x, in_box.m_extension.y));

		if (intersection_segment_quad(ptA, ptB, backFace, out_hit))
		{
			return  INTERSECTION;
		}
	}

	return NO_INTERSECTION;
}
*/


bool intersect_segment_voronol_region(const Vector3f& ptA, const Vector3f& ptB, const Quad& in_quad, const Box& in_box, Hit& out_hit)
{
	Quad shortQuad = Quad(in_quad.m_center, in_quad.m_rotation, in_quad.m_extension);
	shortQuad.m_extension.x = Maths::max(in_quad.m_extension.x - in_box.m_roundness, in_box.m_roundness);
	shortQuad.m_extension.y = Maths::max(in_quad.m_extension.y - in_box.m_roundness, in_box.m_roundness);

	if (intersection_segment_quad(ptA, ptB, shortQuad, out_hit)) return INTERSECTION;


	Vector3f i = vector3RotateByQuaternion({ 1.f,0.f,0.f }, in_quad.m_rotation);
	Vector3f j = vector3RotateByQuaternion({ 0.f,1.f,0.f }, in_quad.m_rotation);
	Vector3f k = vector3RotateByQuaternion({ 0.f,0.f,1.f }, in_quad.m_rotation);

	// INITIAL
	// 
	/*
	Capsule cap;
	Vector3f OI = out_hit.point - in_quad.m_center;
	float dot_I_OI = dotProduct(i, OI);
	float dot_K_OI = dotProduct(k, OI);

	if (fabsf(dot_I_OI) > fabsf(dot_K_OI))
	{
		//	Is a vertical capsule
		//	Left or Right?
		float sign = dot_I_OI > 0.f ? 1.f : -1.f;
		cap = Capsule(shortQuad.m_center - j * radius + sign * i * shortQuad.m_extension.x, shortQuad.m_rotation * quaternionFromEuler(xVector3((float)HALF_PI)), shortQuad.m_extension.y * 2.f, radius);
	}
	else
	{
		//	Is an horizontal capsule
		//	Bottom/back or up/front?
		float sign = dot_K_OI > 0.f ? 1.f : -1.f;
		cap = Capsule(shortQuad.m_center - j * radius + sign * k * shortQuad.m_extension.y, shortQuad.m_rotation * quaternionFromEuler(zVector3((float)HALF_PI)), shortQuad.m_extension.x * 2.f, radius);
	}

	if (intersection_segment_capsule(ptA, ptB, cap, out_hit)) return INTERSECTION;
	*/

	// TESTING ONLY
	// 
	Vector3f OI = out_hit.point - in_quad.m_center;
	float dot_I_OI = dotProduct(OI, i);
	float dot_K_OI = dotProduct(OI, j);

	float signI = dot_I_OI > 0.f ? 1.f : -1.f;
	float signK = dot_K_OI > 0.f ? 1.f : -1.f;
	
	//if (dot_I_OI > in_quad.m_extension.x)
	//{
	//	Capsule cap1 = Capsule(shortQuad.m_center - j * in_box.m_roundness + signI * i * shortQuad.m_extension.x, shortQuad.m_rotation * quaternionFromEuler(xVector3((float)HALF_PI)), shortQuad.m_extension.y * 2.f, in_box.m_roundness);
	//	if (intersection_segment_capsule(ptA, ptB, cap1, out_hit)) return INTERSECTION;
	//}
	//else
	//{
	//	Capsule cap3 = Capsule(shortQuad.m_center - j * in_box.m_roundness + signK * k * shortQuad.m_extension.y, shortQuad.m_rotation * quaternionFromEuler(zVector3((float)HALF_PI)), shortQuad.m_extension.x * 2.f, in_box.m_roundness);
	//	if (intersection_segment_capsule(ptA, ptB, cap3, out_hit)) return INTERSECTION;
	//}
	
	// 3rd cap perpendicular
	Capsule cap = Capsule(in_box.m_center + signI * i * shortQuad.m_extension.x + signK * k * shortQuad.m_extension.y, shortQuad.m_rotation, ((shortQuad.m_center - in_box.m_center).length() - in_box.m_roundness) * 2.0f , in_box.m_roundness);
	if (intersection_segment_capsule(ptA, ptB, cap, out_hit)) return INTERSECTION;
	

	Capsule	cap1 = Capsule(shortQuad.m_center - j * in_box.m_roundness + i * shortQuad.m_extension.x, shortQuad.m_rotation * quaternionFromEuler(xVector3((float)HALF_PI)), shortQuad.m_extension.y * 2.f, in_box.m_roundness);
	Capsule	cap2 = Capsule(shortQuad.m_center - j * in_box.m_roundness - i * shortQuad.m_extension.x, shortQuad.m_rotation * quaternionFromEuler(xVector3((float)HALF_PI)), shortQuad.m_extension.y * 2.f, in_box.m_roundness);
	Capsule	cap3 = Capsule(shortQuad.m_center - j * in_box.m_roundness + k * shortQuad.m_extension.y, shortQuad.m_rotation * quaternionFromEuler(zVector3((float)HALF_PI)), shortQuad.m_extension.x * 2.f, in_box.m_roundness);
	Capsule	cap4 = Capsule(shortQuad.m_center - j * in_box.m_roundness - k * shortQuad.m_extension.y, shortQuad.m_rotation * quaternionFromEuler(zVector3((float)HALF_PI)), shortQuad.m_extension.x * 2.f, in_box.m_roundness);
	
	if (intersection_segment_capsule(ptA, ptB, cap1, out_hit) || intersection_segment_capsule(ptA, ptB, cap2, out_hit) ||
		intersection_segment_capsule(ptA, ptB, cap3, out_hit) || intersection_segment_capsule(ptA, ptB, cap2, out_hit))
		return INTERSECTION;
	
	return NO_INTERSECTION;
}


bool Maths::intersection_segment_box(const Vector3f& ptA, const Vector3f& ptB, const Box& in_rbox, Hit& out_hit)
{
	Vector3f dist = ptB - ptA;
	Vector3f distLonger = dist.normalized() * (in_rbox.m_roundness * 2);
	Vector3f pos = ptA - distLonger;

	Vector3f i = vector3RotateByQuaternion({ 1.f,0.f,0.f }, in_rbox.m_rotation);
	Vector3f j = vector3RotateByQuaternion({ 0.f,1.f,0.f }, in_rbox.m_rotation);
	Vector3f k = vector3RotateByQuaternion({ 0.f,0.f,1.f }, in_rbox.m_rotation);
	Vector3f AB = Vector3f(pos, ptB);

	if (AB.length() == 0)
	{
		return NO_INTERSECTION;
	}

	bool isUnusual1 = false;
	bool isUnusual2 = false;
	bool isUnusual3 = false;

	float projJ = dotProduct(j, AB);
	//	Upper face intersection check
	if (projJ < 0.f)
	{
		Quad upperFace = Quad(in_rbox.m_center + j * (in_rbox.m_extension.y), in_rbox.m_rotation, Vector2f(in_rbox.m_extension.x, in_rbox.m_extension.z));

		if (intersection_segment_quad(pos, ptB, upperFace, out_hit))
		{
			if (intersect_segment_voronol_region(pos, ptB, upperFace, in_rbox, out_hit))
				return INTERSECTION;
		}
	}
	//	Bottom face intersection check
	else if (projJ > 0.f)
	{
		Quad bottomFace = Quad(in_rbox.m_center - j * (in_rbox.m_extension.y), in_rbox.m_rotation * quaternionFromEuler(zVector3((float)PI)), Vector2f(in_rbox.m_extension.x, in_rbox.m_extension.z));

		if (intersection_segment_quad(pos, ptB, bottomFace, out_hit))
		{
			if (intersect_segment_voronol_region(pos, ptB, bottomFace, in_rbox, out_hit))
				return INTERSECTION;
		}
	}
	else
	{
		isUnusual1 = true;
	}

	float projI = dotProduct(i, AB);
	//	Right face intersection check
	if (projI < 0.f)
	{
		Quad rightFace = Quad(in_rbox.m_center + i * (in_rbox.m_extension.x), in_rbox.m_rotation * quaternionFromEuler(zVector3(-(float)HALF_PI)), Vector2f(in_rbox.m_extension.y, in_rbox.m_extension.z));

		if (intersection_segment_quad(pos, ptB, rightFace, out_hit))
		{
			if (intersect_segment_voronol_region(pos, ptB, rightFace, in_rbox, out_hit))
				return INTERSECTION;
		}
	}
	//	Left face intersection check
	else if(projI > 0.f)
	{
		Quad leftFace = Quad(in_rbox.m_center - i * (in_rbox.m_extension.x), in_rbox.m_rotation * quaternionFromEuler(zVector3((float)HALF_PI)), Vector2f(in_rbox.m_extension.y, in_rbox.m_extension.z));

		if (intersection_segment_quad(pos, ptB, leftFace, out_hit))
		{
			if (intersect_segment_voronol_region(pos, ptB, leftFace, in_rbox, out_hit))
				return INTERSECTION;
		}
	}
	else
	{
		isUnusual2 = true;
	}


	float projK = dotProduct(k, AB);
	//	Front face intersection check
	if (projK < 0.f)
	{
		Quad frontFace = Quad(in_rbox.m_center + k * (in_rbox.m_extension.z), in_rbox.m_rotation * quaternionFromEuler(xVector3((float)HALF_PI)), Vector2f(in_rbox.m_extension.x, in_rbox.m_extension.y));

		if (intersection_segment_quad(pos, ptB, frontFace, out_hit))
		{
			if (intersect_segment_voronol_region(pos, ptB, frontFace, in_rbox, out_hit))
				return INTERSECTION;
		}
	}
	//	Back face intersection check
	else if (projK > 0.f)
	{
		Quad backFace = Quad(in_rbox.m_center - k * (in_rbox.m_extension.z), in_rbox.m_rotation * quaternionFromEuler(xVector3(-(float)HALF_PI)), Vector2f(in_rbox.m_extension.x, in_rbox.m_extension.y));

		if (intersection_segment_quad(pos, ptB, backFace, out_hit))
		{
			if (intersect_segment_voronol_region(pos, ptB, backFace, in_rbox, out_hit))
				return INTERSECTION;
		}
	}
	else
	{
		isUnusual3 = true;
	}


	if (isUnusual1 && isUnusual2 && isUnusual3)
	{
		out_hit.normal = (-AB).normalized();
		out_hit.point = ptA;
		return INTERSECTION;
	}

	return NO_INTERSECTION;
}
