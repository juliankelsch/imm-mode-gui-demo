#ifndef CG_FUN_MATRIX4_H
#define CG_FUN_MATRIX4_H

#include "Vector.h"
#include "../Types.h"

// Column-major matrices
struct matrix2
{
    f32 values[4] = {0.0f};
    f32& operator()(u32 Row, u32 Col)
    {
        return values[Col * 2 + Row];
    }
};
struct matrix3
{
    f32 values[9] = {0.0f};
    f32& operator()(u32 Row, u32 Col)
    {
        return values[Col * 3 + Row];
    }
};
struct matrix4
{
    f32 values[16] = {0.0f};
    f32& operator()(u32 Row, u32 Col)
    {
        return values[Col * 4 + Row];
    }
};

matrix2 Matrix( f32 M00, f32 M01,
                f32 M10, f32 M11);
matrix2 Transpose(matrix2 Matrix);
matrix2 IdentityM2();
vector2 Row(matrix2& Matrix, u32 Row);
vector2 Col(matrix2& Matrix, u32 Col);


matrix4 Transpose(matrix4& Matrix);
matrix4 IdentityM4();
matrix4 Matrix(f32 M00, f32 M01, f32 M02, f32 M03,
               f32 M10, f32 M11, f32 M12, f32 M13,
               f32 M20, f32 M21, f32 M22, f32 M23,
               f32 M30, f32 M31, f32 M32, f32 M33);
vector4 Row(matrix4& Matrix, u32 Row);
vector4 Col(matrix4& Matrix, u32 Col);
matrix4 operator*(matrix4& LHS, matrix4& RHS);
vector4 operator*(matrix4& MatrixLHS, vector4 VectorRHS);
matrix4 operator*(matrix4& MatrixLHS, f32 K);
matrix4 operator*(f32 K, matrix4& MatrixRHS);
void Print(matrix4 Matrix);
matrix4 TranslationMatrix(vector3 Translation);
matrix4 ScaleMatrix(vector3 Scale);

matrix4 ChangeOfBasis(vector3 NewXAxis, vector3 NewYAxis, vector3 NewZAxis);

matrix4 OrthographicProjection(f32 Left, f32 Right, f32 Top, f32 Bot, f32 Near, f32 Far);
matrix4 Frustum(f32 Left, f32 Right, f32 Top, f32 Bot, f32 Near, f32 Far);
matrix4 PerspectiveProjection(f32 RadiansFOV, f32 Aspect, f32 Near, f32 Far);
// Note: WARNING! This function only works for OpenGL since the camera is defined to look down the negative z-axis
matrix4 LookAt(vector3 Eye, vector3 Target, vector3 WorldUp);


#endif //CG_FUN_MATRIX4_H
