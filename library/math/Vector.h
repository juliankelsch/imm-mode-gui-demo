#ifndef CG_FUN_VECTOR_H
#define CG_FUN_VECTOR_H

#include "../Types.h"

struct vector2
{
    f32 X, Y;
};

struct vector3
{
    f32 X, Y, Z;
};

struct vector4
{
    f32 X, Y, Z, W;
};

vector2 V(f32 X, f32 Y);
vector2 operator-(vector2 LHS, vector2 RHS);
vector2 operator+(vector2 LHS, vector2 RHS);
vector2 operator*(vector2 Vector, f32 Scalar);
vector2 operator*(f32 Scalar, vector2 Vector);
vector2 operator/(f32 Scalar, vector2 Vector);
vector2 operator/(vector2 Vector, f32 Scalar);
f32     Dot(vector2 LHS, vector2 RHS);
f32     Mag(vector2 Vector);
vector2 Normalize(vector2 Vector);
vector2 operator-(vector2 Vector);
f32     Distance(vector2 A, vector2 B);

vector3 V(f32 X, f32 Y, f32 Z);
vector3 operator-(vector3 A, vector3 B);
vector3 operator+(vector3 A, vector3 B);
vector3 operator*(vector3 Vector, f32 Scalar);
vector3 operator*(f32 Scalar, vector3 Vector);
vector3 operator/(f32 Scalar, vector3 Vector);
vector3 operator/(vector3 Vector, f32 Scalar);
f32     Dot(vector3 LHS, vector3 RHS);
f32     Mag(vector3 Vector);
vector3 Normalize(vector3 Vector);
vector3 operator-(vector3 Vector);
vector3 Cross(vector3 LHS, vector3 RHS);
f32     Distance(vector3 A, vector3 B);

vector4 V(f32 X, f32 Y, f32 Z, f32 W);
vector4 operator-(vector4 LHS, vector3 RHS);
vector4 operator+(vector4 LHS, vector3 RHS);
vector4 operator*(vector4 Vector, f32 Scalar);
vector4 operator*(f32 Scalar, vector4 Vector);
vector4 operator/(f32 Scalar, vector4 Vector);
vector4 operator/(vector4 Vector, f32 Scalar);
f32     Dot(vector4 LHS, vector4 RHS);
f32     Mag(vector4 Vector);
vector4 Normalize(vector4 Vector);
vector4 operator-(vector4 Vector);

vector2 Lerp(vector2 A, vector2 B, f32 T);
vector3 Lerp(vector3 A, vector3 B, f32 T);
vector4 Lerp(vector4 A, vector4 B, f32 T);

void Print(vector2 Vector);
void Print(vector3 Vector);
void Print(vector4 Vector);

#endif //CG_FUN_VECTOR_H
