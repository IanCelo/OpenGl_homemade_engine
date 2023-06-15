#pragma once

#include "Primitives2.h"

namespace  Maths
{
    class Referential2
    {
    public:
        Vector2f   o;
        Vector2f   i;
        Vector2f   j;
        float      m_angle;

        Referential2() { o.x = 0; o.y = 0; i.x = 1; i.y = 0; j.x = 0; j.y = 1; m_angle = 0; };
        Referential2(Vector2f origin, Vector2f axe_i, Vector2f axe_j, float rotation_angle) { o = origin; i = axe_i; j = axe_j; m_angle = rotation_angle; };

        void setAngle(float angle);

        friend std::ostream& operator<<(std::ostream& out, Referential2& ref) { out << " Origin = " << ref.o << ", i = " << ref.i << ", j = " << ref.j << ", Angle =" << ref.m_angle; return out; };
    };


    Vector2f   localToGlobalVector(Referential2 local_ref, Vector2f local_point);

    Vector2f   globalToLocalVector(Referential2 local_ref, Vector2f global_point);

    Vector2f   localToGlobalPoint(Referential2 local_ref, Referential2 global_ref, Vector2f local_point);

    Vector2f   globalToLocalPoint(Referential2 local_ref, Referential2 global_ref, Vector2f global_point);


    ConvexPolygon localToGlobalConvex(ConvexPolygon poly, Referential2 ref);
}