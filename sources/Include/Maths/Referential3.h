#pragma once

#include "Vector3.h"
#include "Quaternion.h"

namespace  Maths
{
    class Referential3
    {
    public:
        Vector3f   o;
        Vector3f   i;
        Vector3f   j;
        Vector3f   k;
        float      m_scale;
        Quaternion      m_angle;

        Referential3() { o.x = 0; o.y = 0; o.z = 0; i.x = 1; i.y = 0; i.z = 0; j.x = 0; j.y = 1; j.z = 0; k.x = 0; k.y = 0; k.z = 1; m_scale = 1; m_angle = quaternionIdentity(); };
        Referential3(Vector3f origin, Vector3f axe_i, Vector3f axe_j, Vector3f axe_k, float scale, Quaternion rotation_angle) { o = origin; i = axe_i; j = axe_j; k = axe_k; m_scale = scale; m_angle = rotation_angle; };

        void setAngle(Quaternion angle);

        void update();
    };

    Vector3f   localToGlobalVector3(Referential3 local_ref, Vector3f local_vector);

    Vector3f   globalToLocalVector3(Referential3 local_ref, Vector3f global_vector);

    Vector3f   localToGlobalPoint3(Referential3 local_ref, Vector3f local_point);

    Vector3f   globalToLocalPoint3(Referential3 local_ref, Vector3f global_point);
}