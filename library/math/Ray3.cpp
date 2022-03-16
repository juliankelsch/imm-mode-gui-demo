//
// Created by julia on 3/12/2021.
//

#include "Ray3.h"
#include "CGMath.h"

bool IntersectsSphere(ray3 Ray, vector3 Center, f32 Radius)
{
    vector3 L = Ray.Origin - Center;
    f32 A = Dot(Ray.Direction, Ray.Direction);
    f32 B = 2.0f * Dot(Ray.Direction, L);
    f32 C = Dot(L, L) - Radius * Radius;
    f32 T0, T1;
    if(!SolveQuadraticEquation(A, B, C, &T0, &T1))
        return false;

    return T0 > 0.0f || T1 > 0.0f;
}

