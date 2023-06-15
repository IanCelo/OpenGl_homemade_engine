
#include <Maths/Random.hpp>

namespace Maths 
{
    float random()
    {
        return (rand() % RAND_MAX) / (float)RAND_MAX;
    }

    float randRange(float min, float max)
    {
        return Maths::lerp(random(), min, max);
    }

    Vector4f randColor(const Vector4f& min, const Vector4f& max)
    {
        Vector4f color;

        color.x = randRange(min.x, max.x);
        color.y = randRange(min.y, max.y);
        color.z = randRange(min.z, max.z);
        color.a = randRange(min.a, max.a);

        return color;
    }

    Vector3f randCircle(float radius)
    {
        Vector2f result{ 0.f, 0.f };
        float randAngle = randRange(0.f, 360.f);
        float randDistance = randRange(0.f, radius);

        result.x = cos(randAngle);
        result.y = sin(randAngle);
        result = result.normalized() * randDistance;

        return { result.x, result.y, 0.f };
    }

    Vector3f randCircle(float angle, float radius, float z)
    {
        Vector2f result{ 0.f, 0.f };
        float randDistance = randRange(0.f, radius);

        result.x = cos(angle);
        result.y = sin(angle);
        result = result.normalized() * randDistance;

        return { result.x, result.y, z };
    }

    // Return direction in a cone
    Vector3f randCone(Vector2f zpos, float baseRadius, float endRadius)
    {
        float randomAngle = randRange(0.f, 360.f);
        Vector3f startPos = randCircle(randomAngle, baseRadius, zpos.x);
        Vector3f endPos = randCircle(randomAngle, endRadius, zpos.y);

        return endPos - startPos;
    }
}