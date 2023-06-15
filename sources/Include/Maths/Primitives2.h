#pragma once
#include "Vector2.h"
#include <vector>

namespace  Maths
{
    class Line
    {
    public:
        Vector2f director;
        Vector2f point;

        //CONSTRUCTORS ~
        Line() {};
        Line(Vector2f dir, Vector2f pt) { director = dir; point = pt; };
        ~Line() {};

        //FUNCTION

    };

    class Segment
    {
    public:
        Vector2f  a;
        Vector2f  b;

        //CONSTRUCTORS ~
        Segment() {};
        Segment(Vector2f pt_A, Vector2f pt_B) { a = pt_A; b = pt_B; };
        ~Segment() {};


        //FUNCTION
        Vector2f getNormal()
        {
            return Vector2f(a, b).normalVector();
        }
    };

    class Circle
    {
    public:
        Vector2f o;     //center
        float radius;

        //CONSTRUCTORS ~
        Circle() { o = { 0,0 }; radius = 1; };
        Circle(Vector2f center, float rad) { o = center; radius = rad; };
        ~Circle() {};

        //FUNCTION

    };

    class Box
    {
    public:
        Vector2f o;     //center
        float length;
        float width;

        //CONSTRUCTORS ~
        Box() { width = 1; length = 1; o = { 0,0 }; };
        Box(Vector2f center, float len, float wid) { o = center; length = len; width = wid; };
        Box(Vector2f center, float flt) { o = center; length = flt; width = flt; };
        Box(Vector2f center, float x_min, float x_max, float y_min, float y_max)
        {
            o = center;
            length = (Vector2f(x_max, 0) - Vector2f(x_min, 0)).length();
            width = (Vector2f(0, y_max) - Vector2f(0, y_min)).length();
        };
        ~Box() {};



        //FUNCTION

        Vector2f getPoint(int x_direction, int y_direction)
        {
            x_direction = (x_direction < 0 ? -1 : 1);
            y_direction = (y_direction < 0 ? -1 : 1);
            Vector2f point((o.x + width / 2) * x_direction, (o.y + length / 2) * y_direction);
            return point;
        }
    };



    class ConvexPolygon
    {
    public:
        ConvexPolygon() {};
        ~ConvexPolygon() {};

        std::vector<Vector2f> points;
        std::vector<Segment> segments;

        void makeSegments()
        {
            segments.clear();
            size_t size = points.size();
            for (size_t index = 0; index < size; index++)
            {
                Vector2f currentPoint = this->points[index];
                Vector2f nextPoint = this->points[(index + 1) % size];

                this->segments.push_back(Segment(currentPoint, nextPoint));
            }
        }
    };

}