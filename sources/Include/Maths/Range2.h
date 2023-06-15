#pragma once

#include "primitives2.h"

namespace  Maths
{
    class Range2
    {
    public:
        //CONSTRUCTORS ~
        Range2() { min = 0; max = 0; };
        Range2(float a, float b) { min = (a < b ? a : b); max = (a > b ? a : b); };
        Range2(float a) { min = a; max = a; };
        ~Range2() {};

        //VARIABLES
        float min;
        float max;

        //FUNCTIONS ~
        Range2& change(float value) { this->min = (this->min < value ? this->min : value); this->max = (this->max > value ? this->max : value); return *this; };
        bool    areInterfering(Range2 range)   const { return !(this->min > range.max || this->max < range.min); };

        //OPERATORS ~
        Range2 operator+(Range2 range) const { Range2 new_range((this->min < range.min ? min : range.min), (this->max > range.max ? max : range.max)); return new_range; };
        friend std::ostream& operator<<(std::ostream& out, Range2& range) { out << " (min: " << range.min << "max: " << range.max << ") "; return out; };

    };

    Range2 range2Point(Vector2f point, Vector2f axe);

    Range2 range2Segment(Segment seg, Vector2f axe);

    Range2 range2Circle(Circle cir, Vector2f axe);

    Range2 range2Box(Box box, Vector2f axe);
}