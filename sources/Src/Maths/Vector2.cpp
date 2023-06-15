#include <cmath>

#include "Maths/Vector2.h"
#include "Maths/Range2.h"
#include "Maths/Utils.h"


Maths::Vector2f::Vector2f()
{
    this->x = 0;
    this->y = 0;
}

Maths::Vector2f::Vector2f(float value)
{
    this->x = value;
    this->y = value;
}

Maths::Vector2f::Vector2f(float x_value,float y_value)
{
    this->x = x_value;
    this->y = y_value;
}

Maths::Vector2f::Vector2f(Vector2f vectA, Vector2f vectB)
{
    this->x = vectB.x - vectA.x;
    this->y = vectB.y - vectA.y;
}

float Maths::Vector2f::length()
{
    return sqrt(this->x*this->x + this->y*this->y);
}

float Maths::Vector2f::squareLength() const
{
    return (this->x * this->x + this->y * this->y);
}
    
Maths::Vector2f Maths::Vector2f::normalized()
{
    Vector2f unit_vector;
    float length = this->length();
    if (length != 0)
    {
        unit_vector = { this->x / length, this->y / length };
        return unit_vector;
    }
    return unit_vector;
}

float Maths::Vector2f::dot(Vector2f vect)
{
    return  vect.x * this->x + vect.y * this->y;
}


Maths::Vector2f Maths::Vector2f::opposite()
{
    return (*this)*(-1.f);
}

float Maths::Vector2f::distance(Vector2f pos)
{
    Vector2f result(pos.x - this->x, pos.y - this->y);
    return result.length();
}

double Maths::Vector2f::angle(Vector2f vect)
{
    return acos(this->dot(vect.normalized()));
}

bool Maths::Vector2f::compare(Vector2f vect)
{
    return (this->distance(vect) < EPSILON);
}


//ANGLE APPLICATIONS

Maths::Vector2f    Maths::Vector2f::rotate(float angle)
{
    float x_result = cos(angle) * this->x - sin(angle) * this->y;
    float y_result = sin(angle) * this->x + cos(angle) * this->y;

    this->x = x_result;
    this->y = y_result;

    return *this;
}

Maths::Vector2f    Maths::Vector2f::rotatePoint(Vector2f center, float angle)
{
    Vector2f vect = *this - center; 
    return vect.rotate(angle) + center;
}

Maths::Vector2f    Maths::Vector2f::rotate90()
{
    Vector2f vect(-(this->y), this->x);
    return vect;
}

Maths::Vector2f   Maths::Vector2f::normalVector()
{
    Vector2f vect = this->normalized();
    return vect.rotate90();
}


bool    Maths::operator==(const Vector2f& vec1, const Vector2f& vec2) { return (vec1.x == vec2.x && vec1.y == vec2.y); };
bool    Maths::operator!=(const Vector2f& vec1, const Vector2f& vec2) { return !(vec1 == vec2); };