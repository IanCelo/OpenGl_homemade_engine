#include <iostream>

#include "Maths/Matrix.h"


using namespace Maths;


/*========================    Matrix 4    =========================*/

Mat4x4  Maths::scale(const Vector3f scale)
{
    return {
        scale.x, 0.f, 0.f, 0.f,
        0.f, scale.y, 0.f, 0.f,
        0.f, 0.f, scale.z, 0.f,
        0.f, 0.f, 0.f, 1.f,
    };
}

//  Pitch rotation
Mat4x4  Maths::rotateX(const float angleRadians)
{
    float c = cosf(angleRadians);
    float s = sinf(angleRadians);

    //  Return 4x4 matrix
    return {
        1.f ,   0.f ,   0.f ,   0.f ,
        0.f ,   c   ,   s   ,   0.f ,
        0.f ,  -s   ,   c   ,   0.f ,
        0.f ,   0.f ,   0.f ,   1.f ,
    };
}

//  Yaw rotation
Mat4x4  Maths::rotateY(const float angleRadians)
{
    float c = cosf(angleRadians);
    float s = sinf(angleRadians);

    //  Return 4x4 matrix
    return {
        c   ,   0.f ,  -s   ,   0.f ,
        0.f ,   1.f ,   0.f ,   0.f ,
        s   ,   0.f ,   c   ,   0.f ,
        0.f ,   0.f ,   0.f ,   1.f ,
    };
}

//  Roll rotation
Mat4x4  Maths::rotateZ(const float angleRadians)
{
    float c = cosf(angleRadians);
    float s = sinf(angleRadians);


    //  Return 4x4 matrix
    return {
        c   ,   s   ,   0.f ,   0.f ,
       -s   ,   c   ,   0.f ,   0.f ,
        0.f ,   0.f ,   1.f ,   0.f ,
        0.f ,   0.f ,   0.f ,   1.f ,
    };
}

//  Translation
Mat4x4  Maths::translate(Vector3f a)
{
    //  Return 4x4 matrix
    return {
        1.f ,   0.f ,   0.f ,   0.f ,
        0.f ,   1.f ,   0.f ,   0.f ,
        0.f ,   0.f ,   1.f ,   0.f ,
        a.x ,   a.y ,   a.z ,   1.f ,
    };
}

//  Frustum
Mat4x4  Maths::frustum(const float left, const float right, const float bottom, const float top, const float near, const float far)
{
    float _near = (2.f * near);
    float _top_bottom = 1 / (top - bottom);
    float _right_left = 1 / (right - left);
    float _far_near = 1 / (far - near);

    float a = _near * _right_left;
    float b = _near * _top_bottom;

    float c = (right + left) * _right_left;
    float d = (top + bottom) * _top_bottom;

    float e = -(far + near) * _far_near;
    float f = -(2.f * far * near) * _far_near;

    //  Return 4x4 matrix
    return {
        a   ,   0.f ,   0.f ,   0.f ,
        0.f ,   b   ,   0.f ,   0.f ,
        c   ,   d   ,   e   ,  -1.f ,
        0.f ,   0.f ,   f   ,   0.f ,
    };
}

//  Perspective
Mat4x4  Maths::perspective(const float fovY, const float aspect, const float near, const float far)
{
    float top = near * tanf(fovY * .5f);
    float right = top * aspect;

    return frustum(-right, right, -top, top, near, far);
}

//  Orthographic
Mat4x4  Maths::Orthographic(const float left, const float right, const float bottom, const float top, const float near, const float far)
{
    float _right_left = 1 / (right - left);
    float _top_bottom = 1 / (top - bottom);
    float _far_near = 1 / (far - near);

    float a = 2 * _right_left;
    float b = 2 * _top_bottom;
    float c = -2 * _far_near;

    float d = -(right + left) * _right_left;
    float e = -(top + bottom) * _top_bottom;
    float f = -(far + near) * _far_near;

    //  Return 4x4 matrix
    return {
        a   ,   0.f ,   0.f ,   0.f ,
        0.f ,   b   ,   0.f ,   0.f ,
        0.f ,   0.f ,   c   ,   0.f ,
        d   ,   e   ,   f   ,   1.f ,
    };
}