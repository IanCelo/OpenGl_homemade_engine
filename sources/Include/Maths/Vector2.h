#pragma once

#include <iostream>

#define EPSILON 0.0001f

namespace Maths
{
    union Vector2f
    {
        //	Variables
        //	---------
        float	c[2];
        struct { float x; float y; };

        //	Constructors
        //	------------

        Vector2f();
        Vector2f(float);
        Vector2f(float, float);
        Vector2f(Vector2f, Vector2f);

        ~Vector2f() {};


        //	Functions
        //	---------

        //  Compare current Vector2f with another
        //  Parameters : Vector2f vect
        //  --------------------------
        bool    compare(Vector2f vect);

        //  Return length of the current Vector2f
        //  Parameters : none
        //  -----------------
        float   length();

        //  Return the square length of the current vector2f
        //  Parameters : none
        //  -----------------
        float   squareLength() const;

        //  Return Dot product between current vector and another one
        //  need to be changed
        float   dot(Vector2f);

        double   angle(Vector2f);

        float   distance(Vector2f);

        Vector2f    normalized();
        Vector2f    opposite();
        Vector2f    rotate(float);
        Vector2f    rotatePoint(Vector2f, float);
        Vector2f    rotate90();
        Vector2f    normalVector();

        //	Operators
        //	---------

        Vector2f    operator-(Vector2f vect) const { Vector2f result(this->x - vect.x, this->y - vect.y); return result; };
        Vector2f    operator-(float value) const { Vector2f result(this->x - value, this->y - value); return result; };
        Vector2f    operator+(Vector2f vect) const { Vector2f result(this->x + vect.x, this->y + vect.y); return result; };
        Vector2f    operator+(float value) const { Vector2f result(this->x + value, this->y + value); return result; };
        Vector2f    operator*(float scale) const { Vector2f result(this->x * scale, this->y * scale); return result; };

        friend std::ostream& operator<<(std::ostream& out, Vector2f& vect) { out << " (" << vect.x << "," << vect.y << ") "; return out; };

    };

    bool    operator==(const Vector2f& vec1, const Vector2f& vec2);
    bool    operator!=(const Vector2f& vec1, const Vector2f& vec2);
}
