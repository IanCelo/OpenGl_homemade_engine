#pragma once

#include <stdlib.h>
#include "Utils.h"
#include "Vector4.h"

namespace Maths 
{
	float random();
	float randRange(float min, float max);
	Vector4f randColor(const Vector4f& min, const Vector4f& max);
	Vector3f randCircle(float radius = 1.f);
	Vector3f randCircle(float angle, float radius, float z);
	Vector3f randCone(Vector2f zpos, float baseRadius = 1.f, float endRadius = 2.f);
}