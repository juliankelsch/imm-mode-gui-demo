
#ifndef PROCEDURALWORLD_RAY3_H
#define PROCEDURALWORLD_RAY3_H

#include "Vector.h"

struct ray3
{
    vector3 Origin;
    vector3 Direction;
};

ray3 Ray(vector3 Origin, vector3 Direction);
bool IntersectsSphere(ray3 Ray, vector3 Center, f32 Radius);
bool IntersectsSphere(ray3 Ray, vector3 Center, f32 Radius, vector3* ClosestIntersection);
// Note: Returns the number of Intersections. If the return value is 0, there is no intersection.
// If the number is 1 the intersection is stored in Intersection0 and Intersection1 is not touched.
u32 IntersectsSphere(ray3 Ray, vector3 Center, f32 Radius, vector3* Intersection0, vector3* Intersection1);

#endif //PROCEDURALWORLD_RAY3_H
