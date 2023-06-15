#include "Maths/Referential2.h"

using namespace Maths;

//  ==================  //
//      REFERENTIAL     //
//  ==================  //

void Maths::Referential2::setAngle(float angle)
{
    j.rotate(angle);
    i.rotate(angle);
}

// --------------- Vector ---------------

//      Local -> Global
Vector2f Maths::localToGlobalVector(Referential2 local_ref, Vector2f local_point)
{
    return local_ref.o + local_ref.i*local_point.x + local_ref.j*local_point.y;
    //          i'(WORLD)           x'(LOCAL)       j'(WORLD)      y'(LOCAL)
}

//      Global -> Local 
Vector2f Maths::globalToLocalVector(Referential2 local_ref, Vector2f global_point)
{
    Vector2f vect(global_point.dot(local_ref.i.normalized()), global_point.dot(local_ref.j.normalized()));
    return vect;
}

// --------------- Position ---------------

//      Local -> Global
Vector2f Maths::localToGlobalPoint(Referential2 local_ref, Referential2 global_ref, Vector2f local_point)
{
    Vector2f vector_origins = local_ref.o-global_ref.o;                 //vector o (global) o' (global)
    Vector2f vector_local   = (local_ref.o+ local_point)-local_ref.o;   //vector o (local) point P (local)
 
    return (vector_origins+vector_local) + localToGlobalVector(local_ref, local_point);
}

//      Global -> Local 
Vector2f Maths::globalToLocalPoint(Referential2 local_ref, Referential2 global_ref, Vector2f global_point)
{

    Vector2f vector_origins = local_ref.o-global_ref.o;         //vector o (global) o' (global)
    Vector2f vector_global  = global_point - vector_origins;    //vector o (global) point p (global)

    Vector2f vect(local_ref.i.dot(vector_global),local_ref.j.dot(vector_global));
    return vect;
}

ConvexPolygon Maths::localToGlobalConvex(ConvexPolygon poly, Referential2 ref)
{
    ConvexPolygon global_poly = ConvexPolygon();
    for (int i = 0; i < poly.points.size(); i++)
    {
        global_poly.points.push_back(localToGlobalVector(ref, poly.points[i]));
    }
    return global_poly;
}