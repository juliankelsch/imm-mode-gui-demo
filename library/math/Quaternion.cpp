#include "Quaternion.h"

#include <cmath>
#include <cstdio>

quaternion operator+(quaternion LHS, quaternion RHS)
{
    quaternion Result = {LHS.S + RHS.S, LHS.V + RHS.V};
    return Result;
}

quaternion operator-(quaternion LHS, quaternion RHS)
{
    quaternion Result = {LHS.S - RHS.S, LHS.V - RHS.V};
    return Result;
}

quaternion operator*(quaternion QuaternionLHS, f32 K)
{
    quaternion Result = {QuaternionLHS.S * K, QuaternionLHS.V * K};
    return Result;
}

quaternion operator*(f32 K, quaternion QuaternionRHS)
{
    quaternion Result = {QuaternionRHS.S * K, QuaternionRHS.V * K};
    return Result;
}

quaternion operator/(f32 K, quaternion QuaternionRHS)
{
    return QuaternionRHS * (1.0f / K);
}

quaternion operator/(quaternion QuaternionLHS, f32 K)
{
    return QuaternionLHS * (1.0f / K);
}

f32 Length(quaternion Quaternion)
{
    return Quaternion.S * Quaternion.S + Dot(Quaternion.V, Quaternion.V);
}

quaternion Normalize(quaternion Quaternion)
{
    quaternion Result = Quaternion / Length(Quaternion);
    return Result;
}

quaternion Conjugate(quaternion Quaternion)
{
    quaternion Result = {Quaternion.S, -Quaternion.V};
    return Result;
}

f32 Dot(quaternion LHS, quaternion RHS)
{
    return LHS.S * RHS.S + Dot(LHS.V, RHS.V);
}


quaternion Inverse(quaternion Quaternion)
{
    return Conjugate(Quaternion) / (Quaternion.S * Quaternion.S + Dot(Quaternion.V, Quaternion.V));
}

quaternion AxisAngleAny(f32 Radians, vector3 Axis)
{
    quaternion Result = {cosf(Radians / 2.0f), sinf(Radians / 2.0f) * Normalize(Axis)};
    return Result;
}

quaternion AxisAngle(f32 Radians, vector3 UnitAxis)
{
    quaternion Result = {cosf(Radians / 2.0f), sinf(Radians / 2.0f) * UnitAxis};
    return Result;
}

quaternion operator*(quaternion LHS, quaternion RHS)
{
    quaternion Product = {LHS.S * RHS.S - Dot(LHS.V, RHS.V), (LHS.S * RHS.V) + (RHS.S * LHS.V) + Cross(LHS.V, RHS.V)};
    return Product;
}

vector3 Rotate(vector3 Point, quaternion RotationQuaternion)
{
    quaternion PointQuaternion = {0, Point};
    return (RotationQuaternion * PointQuaternion * Conjugate(RotationQuaternion)).V;
}

quaternion operator-(quaternion Quaternion)
{
    return Quaternion * -1.0f;
}

quaternion Slerp(quaternion A, quaternion B, f32 T)
{
    f32 theta = 2.0f * acosf(Dot(A, B));
    f32 sin_theta = sinf(theta);

    if(Length(A - B) > Length(A + B))
        B = -B;

    return ((sinf((1 - T) * theta)) / sin_theta) * A + (sinf(T * theta) / sin_theta) * -B;
}

matrix4 ToMatrix(quaternion Quaternion)
{
    f32 S = Quaternion.S, X = Quaternion.V.X, Y = Quaternion.V.Y, Z = Quaternion.V.Z;
    return Matrix(1.0f - 2.0f * (Y * Y + X * X), 2.0f * (X * Y - S * Z), 2.0f * (S * Y + X * Z), 0.0f,
                  2.0f * (X * Y + S * Z), 1.0f - 2.0f * (X * X + Z * Z), 2.0f * (Y * Z - S * X), 0.0f,
                  2.0f * (X * Z - S * Y), 2.0f * (S * X + Y * Z), 1.0f - 2.0f * (X * X + Y * Y), 0.0f,
                  0.0f, 0.0f, 0.0f, 1.0f);
}

quaternion FromMatrix(matrix4 Matrix)
{
    f32 S = sqrtf(Matrix(0, 0) + Matrix(1, 1) + Matrix(2, 2) + 1) / 2.0f;
    f32 X = (Matrix(2, 1) - Matrix(1, 2)) / (4.0f * S);
    f32 Y = (Matrix(0, 2) - Matrix(2, 0)) / (4.0f * S);
    f32 Z = (Matrix(1, 0) - Matrix(0, 1)) / (4.0f * S);

    quaternion Result = {S, {X, Y, Z}};
    return Result;
}

void Print(quaternion Quaternion)
{
    printf("[%.2f, [%.2f, %.2f, %.2f]]", Quaternion.S, Quaternion.V.X, Quaternion.V.Y, Quaternion.V.Z);
}
