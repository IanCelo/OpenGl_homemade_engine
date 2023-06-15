#pragma once
#include "Shapes3.h"

namespace  Maths
{
	//	Structure that contains the intersection point and the normal of the intersection
	//	---------------------------------------------------------------------------------
	struct Hit
	{
		Vector3f point;
		Vector3f normal;
	};

	//	Intersection Funcions between segment and shapes in 3D
	//	In the following functon, ptA and ptB form the segment [AB],
	//	The 'Hit' variables is a referenced structure, so it can be modified, 
	//	it returns the intersection point and the normal from the shape on the point
	//	----------------------------------------------------------------------------


	//	Check intersection between a segment and a plane
	//	Parameters : const Vector3f& ptA, const Vector3f& ptB, const Plane& in_plane, Hit& out_hit
	//	------------------------------------------------------------------------------------------
	bool intersection_segment_plane(const Vector3f& ptA, const Vector3f& ptB, const Plane& in_plane, Hit& out_hit);


	//	Check intersection between a segment and a disk
	//	Parameters : const Vector3f& ptA, const Vector3f& ptB, const Disk& in_disk, Hit& out_hit
	//	----------------------------------------------------------------------------------------
	bool intersection_segment_disk(const Vector3f& ptA, const Vector3f& ptB, const Disk& in_disk, Hit& out_hit);


	//	Check intersection between a segment and a sphere
	//	Parameters : const Vector3f& ptA, const Vector3f& ptB, const Sphere& in_sphere, Hit& out_hit
	//	--------------------------------------------------------------------------------------------
	bool intersection_segment_sphere(const Vector3f& ptA, const Vector3f& ptB, const Sphere& in_sphere, Hit& out_hit);


	//	Check intersection between a segment and a cylinder with infinite height
	//	Parameters : const Vector3f& ptA, const Vector3f& ptB, const Cylinder& in_cyl, Hit& out_hit
	//	-------------------------------------------------------------------------------------------
	bool intersection_segment_inf_cylinder(const Vector3f& ptA, const Vector3f& ptB, const  Cylinder& in_cyl, Hit& out_hit);


	//	Check intersection between a segment and a cylinder
	//	Parameters : const Vector3f& ptA, const Vector3f& ptB, const Cylinder& in_cyl, Hit& out_hit
	//	-------------------------------------------------------------------------------------------
	bool intersection_segment_cylinder(const Vector3f& ptA, const Vector3f& ptB, const Cylinder& in_cyl, Hit& out_hit);


	//	Check intersection between a segment and a capsule
	//	Parameters : const Vector3f& ptA, const Vector3f& ptB, const Capsule& in_cap, Hit& out_hit
	//	------------------------------------------------------------------------------------------
	bool intersection_segment_capsule(const Vector3f& ptA, const Vector3f& ptB, const Capsule& in_cap, Hit& out_hit);


	//	Check intersection between a segment and a quad
	//	Parameters : const Vector3f& ptA, const Vector3f& ptB, const Quad& in_quad, Hit& out_hit
	//	----------------------------------------------------------------------------------------
	bool intersection_segment_quad(const Vector3f& ptA, const Vector3f& ptB, const Quad& in_quad, Hit& out_hit);


	//	Check intersection between a segment and a box
	//	Parameters : const Vector3f& ptA, const Vector3f& ptB, const Box& in_box, Hit& out_hit
	//	--------------------------------------------------------------------------------------
	bool intersection_segment_box(const Vector3f& ptA, const Vector3f& ptB, const Box& in_box, Hit& out_hit);


	//	Check intersection between a segment and a rounded-box
	//	Parameters : const Vector3f& ptA, const Vector3f& ptB, const RoundedBox& in_box, Hit& out_hit
	//	---------------------------------------------------------------------------------------------
	//bool intersection_segment_roundedbox(const Vector3f& ptA, const Vector3f& ptB, const RoundedBox& in_box, Hit& out_hit);


	//	Line intersection
	//	In the following functon, ptA and ptB form the line (AB)
	//	--------------------------------------------------------

	//	Check intersection between a line and a plane
	//	Parameters : const Vector3f& ptA, const Vector3f& ptB, const Plane& in_plane, Hit& out_hit
	//	------------------------------------------------------------------------------------------
	bool intersection_line_plane(const Vector3f& ptA, const Vector3f& ptB, const Plane& in_plane, Hit& out_hit);
}