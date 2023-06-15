#include <cmath>

#include "Maths/Intersection2.h"
#include "Maths/Utils.h"

//  Is there an intersection between those two points?
bool Maths::intersectionPointPoint(Vector2f point_A, Vector2f point_B)
{
    return (pow(point_A.x-point_B.x,2)+pow(point_A.y-point_B.y,2) < EPSILON);
}

//  Is there an intersection between this point and this line?
bool Maths::intersectionPointLine(Vector2f point, Line line)
{
    Vector2f vectAP = line.point-point;
    Vector2f normal_vect = (line.director.normalized()).rotate90(); 
    return (abs(vectAP.dot(normal_vect)) < EPSILON);
}

//  Is there an intersection between this point and this segment?
bool Maths::intersectionPointSegment(Vector2f point, Segment seg)
{
    Line line(seg.a, (seg.b-seg.a).normalized() );
    Range2 rangePnt = range2Point(point, line.director);
    Range2 rangeSeg = range2Segment(seg, line.director);
    
    return rangePnt.areInterfering(rangeSeg);
}

//  Is there an intersection between this point and this circle?
bool Maths::intersectionPointCircle(Vector2f point, Circle cir)
{
    float length_OP = (point-cir.o).length();

    return (pow(length_OP,2) <= pow(cir.radius,2));
}

//  Is there an intersection between this point and this box?
bool Maths::intersectionPointBox(Vector2f point, Box box)
{
    float half_width = box.width/2;
    float half_length = box.length/2;
    return (abs(point.x-box.o.x) < half_width && abs(point.y-box.o.y) < half_length);
}

//  Is there an intersection between those two lines?
bool Maths::intersectionLineLine(Line line_A, Line line_B)
{
    Vector2f normal = line_A.director.rotate90();

    if (normal.dot(line_B.director)!= 0)
    {
        return true;
    }  
    
    return (intersectionPointLine(line_A.point, line_B));
}

//  Is there an intersection between this line and this segment?
bool Maths::intersectionLineSegment(Line line_A, Segment segment)
{
    Vector2f vectPA = line_A.point-segment.a;
    Vector2f vectPB = line_A.point-segment.b;

    Vector2f normal = line_A.director.normalized().rotate90();

    if ((vectPA.dot(normal) * vectPB.dot(normal)) > 0)
    {
        return false;
    }
        
    
    Line line_B(segment.a, (segment.b-segment.a).normalized());
    return intersectionLineLine(line_A, line_B);
}

//  Is there an intersection between this line and this cirlce?
bool Maths::intersectionLineCircle(Line line, Circle circle)
{
    Vector2f vectAO = line.point-circle.o;
    Vector2f normal = line.director.rotate90();
    
    return (abs(vectAO.dot(normal)) <= circle.radius);
}

//  Is there an intersection between those two segments?
bool Maths::intersectionSegmentSegment(Segment segment_A, Segment segment_B)
{
    Vector2f dir_A = (segment_A.b-segment_A.a).normalized();
    Vector2f dir_B = (segment_B.b-segment_B.a).normalized();

    Line line_A(segment_A.a, dir_A);
    Line line_B(segment_B.a, dir_B);

    if (intersectionLineSegment(line_A, segment_B) && intersectionLineSegment(line_B, segment_A))
    {
        return range2Segment(segment_A,dir_A).areInterfering(range2Segment(segment_B,dir_A));
    }return false;
}

//  Is there an intersection between this segment and this circle?
bool Maths::intersectionSegmentCircle(Segment segment, Circle circle)
{
    Line line(segment.a,(segment.b-segment.a).normalized() );
    if (!intersectionLineCircle(line,circle))
        return false;

    Vector2f OA = (segment.a-circle.o);
    if (OA.length() < circle.radius)
        return true;

    Vector2f OB = segment.b-circle.o;
    if (OB.length() < circle.radius)
        return true;

    return intersectionPointSegment(circle.o, segment);
}


bool Maths::intersectionCircleCircle(Circle circle_A, Circle circle_B)
{
    return ((circle_B.o-circle_A.o).length() <= (circle_A.radius + circle_B.radius));
}

//  Is there an intersection between this circle and this box?
bool Maths::intersectionCircleBox(Circle circle, Box box)
{
    if (intersectionPointBox(circle.o, box))
        return true;
    
    //nearest point of the box from the O() of circle
    Vector2f h(Maths::clamp(circle.o.x, box.o.x - box.width / 2, box.o.x + box.width / 2), Maths::clamp(circle.o.y, box.o.y - box.length / 2, box.o.y + box.length / 2));
    
    //vector circle O() to h
    Vector2f CH = circle.o-h;

    return circle.radius >= CH.length(); 
}

//  Is there an intersection between two boxes?
bool Maths::intersectionBoxBox(Box box_A, Box box_B)
{
    Range2 box_range_AX((box_A.o.x - box_A.width/2.f),(box_A.o.x + box_A.width/2.f));
    Range2 box_range_AY((box_A.o.y - box_A.length/2.f),(box_A.o.y + box_A.length/2.f));
    
    Range2 box_range_BX((box_B.o.x - box_B.width/2.f),(box_B.o.x + box_B.width/2.f));
    Range2 box_range_BY((box_B.o.y - box_B.length/2.f),(box_B.o.y + box_B.length/2.f));

    return (box_range_AX.areInterfering(box_range_BX) && box_range_AY.areInterfering(box_range_BY));
}


//  Is there an intersection between a convex and a point?
bool Maths::intersectionConvexPoint(ConvexPolygon convex, Vector2f point)
{   
    convex.makeSegments();

    for( int i = 0 ; i < (int)convex.segments.size() ; i++)
    {
        Vector2f normal = convex.segments[i].getNormal();
        Range2 current_range = range2Segment(convex.segments[0],normal);
        for ( int j = 1; j<(int)convex.points.size() ; j++ )
        {
            current_range = current_range+range2Segment(convex.segments[j],normal);
        }
        if(!(current_range.areInterfering(range2Point(point,normal))))
        {
                return false;
        }
    }
    return true;
}

//Test intersection between two convex from one side
bool convexConvexTest(Maths::ConvexPolygon convex1, Maths::ConvexPolygon convex2)
{
    for (int i = 0; i < (int)convex1.segments.size(); i++)
    {
        Maths::Vector2f normal = convex1.segments[i].getNormal();
        Maths::Range2 current_range1 = range2Segment(convex1.segments[0], normal);
        for (int j = 1; j < (int)convex1.points.size(); j++)
            current_range1 = current_range1 + range2Segment(convex1.segments[j], normal);
      
        Maths::Range2 current_range2 = range2Segment(convex2.segments[0], normal);
        for (int j = 1; j < (int)convex2.points.size(); j++)
            current_range2 = current_range2 + range2Segment(convex2.segments[j], normal);
        
        if (!(current_range1.areInterfering(current_range2)))
            return false;
    }
    return true;
}

//  Is there an intersection between two convexes?
bool Maths::intersectionConvexConvex(ConvexPolygon convex1, ConvexPolygon convex2)
{   
    convex1.makeSegments();
    convex2.makeSegments();

    return convexConvexTest(convex1, convex2) && convexConvexTest(convex2, convex1);
}

//  Is there an intersection between a convex and a circle?
bool Maths::intersectionConvexCircle(ConvexPolygon convex, Circle circle)
{   
    convex.makeSegments();

    float min = (convex.points[0] - circle.o).squareLength();
    Vector2f nearest_point = convex.points[0];
     
    for( int i = 0 ; i < (int)convex.segments.size() ; i++)
    {
        Vector2f normal = convex.segments[i].getNormal();
        Range2 current_range1 = range2Segment(convex.segments[0],normal);

        for (int j = 1; j < (int)convex.points.size(); j++)
        {
            current_range1 = current_range1 + range2Segment(convex.segments[j], normal);
            if ((convex.points[j] - circle.o).squareLength() < min)
            {
                min = (convex.points[j] - circle.o).squareLength();
                nearest_point = convex.points[j];
            }
        }
        
        Range2 current_range2 = range2Circle(circle,normal);
        if(!(current_range1.areInterfering(current_range2)))
            return false;   
    }

    
    Vector2f normal = (nearest_point-circle.o).normalized();
    Range2 current_range1 = range2Segment(convex.segments[0], normal);
    Range2 current_range2 = range2Circle(circle, normal);

    for (int i = 1; i < (int)convex.points.size(); i++)
        current_range1 = current_range1 + range2Point(convex.points[i], normal);
    
    if (!(current_range1.areInterfering(current_range2)))
        return false;
    
    return true;
}