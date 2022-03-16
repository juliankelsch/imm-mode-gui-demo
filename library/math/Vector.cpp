//
// Created bY julia on 2/27/2021.
//

#include "Vector.h"

#include <cmath>
#include <cstdio>
#include "CGMath.h"

vector2 operator-(vector2 LHS, vector2 RHS)
{
    vector2 Result = {LHS.X - RHS.X, LHS.Y - RHS.Y};
    return Result;
}

vector2 operator+(vector2 LHS, vector2 RHS)
{
    vector2 Result = {LHS.X + RHS.X, LHS.Y + RHS.Y};
    return Result;
}

vector2 operator*(vector2 Vector, f32 Scalar)
{
    vector2 Result = {Vector.X * Scalar, Vector.Y * Scalar};
    return Result;
}

vector2 operator*(f32 Scalar, vector2 Vector)
{
    vector2 Result = {Vector.X * Scalar, Vector.Y * Scalar};
    return Result;
}

vector2 operator/(f32 Scalar, vector2 Vector)
{
    return Vector * (1.0f / Scalar);
}

vector2 operator/(vector2 Vector, f32 Scalar)
{
    return Vector * (1.0f / Scalar);
}

f32 Dot(vector2 LHS, vector2 RHS)
{
    return LHS.X * RHS.X + LHS.Y * RHS.Y;
}

f32 Mag(vector2 Vector)
{
    return sqrtf(Dot(Vector, Vector));
}

vector2 Normalize(vector2 Vector)
{
    return Vector / Mag(Vector);
}

f32 Distance(vector2 A, vector2 B)
{
   return Mag(A - B);
}

vector2 operator-(vector2 Vector)
{
    return Vector * -1.0f;
}

vector3 operator+(vector3 A, vector3 B)
{
    vector3 Result = {A.X + B.X, A.Y + B.Y, A.Z + B.Z};
    return Result;
}

vector3 operator-(vector3 A, vector3 B)
{
    vector3 Result = {A.X - B.X, A.Y - B.Y, A.Z - B.Z};
    return Result;
}

vector3 operator*(vector3 v, f32 k)
{
    vector3 Result = {v.X * k, v.Y * k, v.Z * k};
    return Result;
}

vector3 operator*(f32 k, vector3 v)
{
    vector3 Result = {v.X * k, v.Y * k, v.Z * k};
    return Result;
}

vector3 operator/(vector3 v, f32 k)
{
    return v * (1.0f / k);
}

vector3 operator/(f32 k, vector3 v)
{
    return v * (1.0f / k);
}

vector3 operator-(vector3 v)
{
    return v * -1.0f;
}

f32 Dot(vector3 a, vector3 b)
{
    return a.X * b.X + a.Y * b.Y + a.Z * b.Z;
}

f32 Mag(vector3 v)
{
    return sqrtf(Dot(v, v));
}

vector3 Normalize(vector3 Vector)
{
    return Vector / Mag(Vector);
}

vector3 Cross(vector3 LHS, vector3 RHS)
{
    vector3 Result = {
            LHS.Y * RHS.Z - RHS.Y * LHS.Z,
            LHS.Z * RHS.X - RHS.Z * LHS.X,
            LHS.X * RHS.Y - RHS.X * LHS.Y
    };
    return Result;
}

f32 Distance(vector3 A, vector3 B)
{
    return Mag(A-B);
}

vector4 operator-(vector4 a, vector4 b)
{
    vector4 Result = {a.X - b.X, a.Y - b.Y, a.Z - b.Z, a.W - b.W};
    return Result;
}

vector4 operator+(vector4 a, vector4 b)
{
    vector4 Result = {a.X + b.X, a.Y + b.Y, a.Z + b.Z, a.W + b.W};
    return Result;
}

vector4 operator*(vector4 v, f32 k)
{
    vector4 Result = {v.X * k, v.Y * k, v.Z * k, v.W * k};
    return Result;
}

vector4 operator*(f32 k, vector4 v)
{
    vector4 Result = {v.X * k, v.Y * k, v.Z * k, v.W * k};
    return Result;
}

vector4 operator/(vector4 v, f32 k)
{
    return v * (1.0f / k);
}

vector4 operator/(f32 k, vector4 v)
{
    return v * (1.0f / k);
}

f32 Dot(vector4 a, vector4 b)
{
    return a.X * b.X + a.Y * b.Y + a.Z * b.Z + a.W * b.W;
}

f32 Mag(vector4 v)
{
    return sqrtf(Dot(v, v));
}

vector4 operator-(vector4 v)
{
    return v * -1.0f;
}

vector2 Lerp(vector2 A, vector2 B, f32 T)
{
    vector2 Result = {Lerp(A.X, B.X, T),
                   Lerp(A.Y, B.Y, T)};
    return Result;
}

vector3 Lerp(vector3 A, vector3 B, f32 T)
{
    vector3 Result = {Lerp(A.X, B.X, T),
                   Lerp(A.Y, B.Y, T),
                   Lerp(A.Z, B.Z, T)};
    return Result;
}

vector4 Lerp(vector4 A, vector4 B, f32 T)
{
    vector4 Result = {Lerp(A.X, B.X, T),
                   Lerp(A.Y, B.Y, T),
                   Lerp(A.Z, B.Z, T),
                   Lerp(A.W, B.W, T)};
    return Result;
}

void Print(vector3 Vector)
{
    printf("[%.2f, %.2f, %.2f]", Vector.X, Vector.Y, Vector.Z);
}

void Print(vector2 Vector)
{
    printf("[%.2f, %.2f]", Vector.X, Vector.Y);
}

void Print(vector4 Vector)
{
    printf("[%.2f, %.2f, %.2f, %.2f]", Vector.X, Vector.Y, Vector.Z, Vector.W);
}
vector2 V(f32 X, f32 Y)
{
    vector2 Result = {X, Y};
    return Result;
}
vector3 V(f32 X, f32 Y, f32 Z)
{
    vector3 Result = {X, Y, Z};
    return Result;
}
vector4 V(f32 X, f32 Y, f32 Z, f32 W)
{
    vector4 Result = {X, Y, Z, W};
    return Result;
}
