#pragma once

#include "Range2.h"


namespace  Maths
{
	bool intersectionPointPoint(Vector2f, Vector2f);

	bool intersectionPointLine(Vector2f, Line);

	bool intersectionPointSegment(Vector2f, Segment);

	bool intersectionPointCircle(Vector2f, Circle);

	bool intersectionPointBox(Vector2f, Box);

	bool intersectionLineLine(Line, Line);

	bool intersectionLineSegment(Line, Segment);

	bool intersectionLineCircle(Line, Circle);

	bool intersectionSegmentSegment(Segment, Segment);

	bool intersectionSegmentCircle(Segment, Circle);

	bool intersectionCircleCircle(Circle, Circle);

	bool intersectionCircleBox(Circle, Box);

	bool intersectionBoxBox(Box, Box);

	bool intersectionConvexPoint(ConvexPolygon, Vector2f);

	bool intersectionConvexConvex(ConvexPolygon, ConvexPolygon);

	bool intersectionConvexCircle(ConvexPolygon, Circle);
}
