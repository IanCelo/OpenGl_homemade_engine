#include "Maths/Referential3.h"

using namespace Maths;

//  ==================  //
//      REFERENTIAL     //
//  ==================  //

void Maths::Referential3::setAngle(Quaternion angle)
{
    k = vector3RotateByQuaternion(k, angle);
    j = vector3RotateByQuaternion(j, angle);
    i = vector3RotateByQuaternion(i, angle);
}

// --------------- Vector ---------------

//      Local -> Global
Vector3f Maths::localToGlobalVector3(Referential3 local_ref, Vector3f local_vector)
{
    return vector3RotateByQuaternion(local_vector, local_ref.m_angle);
}

//      Global -> Local
Vector3f Maths::globalToLocalVector3(Referential3 local_ref, Vector3f global_vector)
{
    return vector3RotateByQuaternion(global_vector, local_ref.m_angle.invert());
}

// --------------- Position ---------------

//      Local -> Global
Vector3f Maths::localToGlobalPoint3(Referential3 local_ref, Vector3f local_point)
{
    Vector3f global_point = vector3RotateByQuaternion(local_point, local_ref.m_angle) + local_ref.o;
    return global_point;
}

//      Global -> Local 
Vector3f Maths::globalToLocalPoint3(Referential3 local_ref, Vector3f global_point)
{
    Vector3f local_point = vector3RotateByQuaternion(global_point - local_ref.o, local_ref.m_angle.invert());
    return local_point;
}

void Referential3::update()
{
    i = vector3RotateByQuaternion(i, m_angle);
    j = vector3RotateByQuaternion(j, m_angle);
    k = vector3RotateByQuaternion(k, m_angle);
}