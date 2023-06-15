#include "Maths/Range2.h"

using namespace Maths;
Range2 Maths::range2Point(Vector2f point, Vector2f axe)
{ 
    Range2 range(point.dot(axe));
    return range;
}

Range2 Maths::range2Segment(Segment seg, Vector2f axe)
{ 
    Range2 range(seg.a.dot(axe), seg.b.dot(axe));
    return range;
}

Range2 Maths::range2Circle(Circle cir, Vector2f axe)
{ 
    Vector2f unit = axe.normalized();
    Segment seg(cir.o - unit*cir.radius,cir.o + unit*cir.radius);

    return range2Segment(seg, axe) ;
}

Range2 Maths::range2Box(Box box, Vector2f axe)
{
    Range2 range_A((box.getPoint(1,-1).dot(axe)),(box.getPoint(1,1).dot(axe)));
    Range2 range_B((box.getPoint(-1,1).dot(axe)),(box.getPoint(-1,-1).dot(axe))); 
    return (range_A+range_B);
}
