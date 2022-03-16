#ifndef CG_FUN_QUATERNION_H
#define CG_FUN_QUATERNION_H

#include "Vector.h"
#include "Matrix.h"

struct quaternion
{
    f32 S; vector3 V;
};

quaternion  operator+(quaternion LHS, quaternion RHS);
quaternion  operator-(quaternion LHS, quaternion RHS);
quaternion  operator*(quaternion QuaternionLHS, f32 K);
quaternion  operator*(f32 K, quaternion QuaternionRHS);
quaternion  operator/(f32 K, quaternion QuaternionRHS);
quaternion  operator/(quaternion QuaternionLHS, f32 K);
quaternion  Normalize(quaternion Quaternion);
quaternion  Conjugate(quaternion Quaternion);
quaternion  Inverse(quaternion Quaternion);
quaternion  AxisAngleAny(f32 Radians, vector3 Axis);
quaternion  AxisAngle(f32 Radians, vector3 UnitAxis);
quaternion  operator*(quaternion LHS, quaternion RHS);
quaternion  operator-(quaternion Quaternion);
quaternion  Slerp(quaternion A, quaternion B, f32 T);
quaternion  FromMatrix(matrix4 Matrix);
vector3     Rotate(vector3 Point, quaternion RotationQuaternion);
f32         Length(quaternion Quaternion);
f32         Dot(quaternion LHS, quaternion RHS);
matrix4     ToMatrix(quaternion Quaternion);

void Print(quaternion Quaternion);

#endif //CG_FUN_QUATERNION_H
