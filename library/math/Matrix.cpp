//
// Created by julia on 2/27/2021.
//

#include "Matrix.h"
#include <cmath>
#include <cstdio>

matrix4 Transpose(matrix4& Matrix)
{
    matrix4 Res;
    for (u32 RowIndex = 0; RowIndex < 4; ++RowIndex) {
        for (u32 ColIndex = 0; ColIndex < 4; ++ColIndex) {
            Res(RowIndex, ColIndex) = Matrix(ColIndex, RowIndex);
        }
    }
    return Res;
}

matrix4 IdentityM4()
{
    return Matrix(1.0f, 0.0f, 0.0f, 0.0f,
                  0.0f, 1.0f, 0.0f, 0.0f,
                  0.0f, 0.0f, 1.0f, 0.0f,
                  0.0f, 0.0f, 0.0f, 1.0f);
}

matrix4 Matrix(f32 M00, f32 M01, f32 M02, f32 M03,
               f32 M10, f32 M11, f32 M12, f32 M13,
               f32 M20, f32 M21, f32 M22, f32 M23,
               f32 M30, f32 M31, f32 M32, f32 M33)
{
    matrix4 NewMatrix;
    NewMatrix(0, 0) = M00;
    NewMatrix(0, 1) = M01;
    NewMatrix(0, 2) = M02;
    NewMatrix(0, 3) = M03;

    NewMatrix(1, 0) = M10;
    NewMatrix(1, 1) = M11;
    NewMatrix(1, 2) = M12;
    NewMatrix(1, 3) = M13;

    NewMatrix(2, 0) = M20;
    NewMatrix(2, 1) = M21;
    NewMatrix(2, 2) = M22;
    NewMatrix(2, 3) = M23;

    NewMatrix(3, 0) = M30;
    NewMatrix(3, 1) = M31;
    NewMatrix(3, 2) = M32;
    NewMatrix(3, 3) = M33;
    return NewMatrix;
}

matrix4 operator*(matrix4& LHS, matrix4& RHS)
{
    matrix4 Product;
    for (u32 RowIndex = 0; RowIndex < 4; ++RowIndex) {
        for (u32 ColIndex = 0; ColIndex < 4; ++ColIndex) {
            Product(RowIndex, ColIndex) = Dot(Row(LHS, RowIndex), Col(RHS, ColIndex));
        }
    }
    return Product;
}

vector4 operator*(matrix4& MatrixLHS, vector4 VectorRHS)
{
    vector4 res = {Dot(Row(MatrixLHS, 0), VectorRHS),
                   Dot(Row(MatrixLHS, 1), VectorRHS),
                   Dot(Row(MatrixLHS, 2), VectorRHS),
                   Dot(Row(MatrixLHS, 3), VectorRHS)};
    return res;
}

vector4 Row(matrix4& Matrix, u32 Row)
{
    vector4 RowVector = {Matrix(Row, 0), Matrix(Row, 1), Matrix(Row, 2), Matrix(Row, 3)};
    return RowVector;
}

vector4 Col(matrix4& Matrix, u32 Col)
{
    vector4 ColVector = {Matrix(0, Col), Matrix(1, Col), Matrix(2, Col), Matrix(3, Col)};
    return ColVector;
}

matrix4 operator*(matrix4& MatrixLHS, f32 K)
{
    matrix4 Result;
    for (u32 i = 0; i < 16; ++i) {
        Result.values[i] = MatrixLHS.values[i] * K;
    }
    return Result;
}

matrix4 operator*(f32 K, matrix4& MatrixRHS)
{
    matrix4 Result;
    for (u32 i = 0; i < 16; ++i) {
        Result.values[i] = MatrixRHS.values[i] * K;
    }
    return Result;
}

void Print(matrix4 Matrix)
{
    printf("{");
    for (u32 i = 0; i < 4; ++i) {
        Print(Row(Matrix, i));
        printf("\n");
    }
    printf("}");
}

matrix4 TranslationMatrix(vector3 Translation)
{
    matrix4 Result = IdentityM4();
    Result(0, 3) = Translation.X;
    Result(1, 3) = Translation.Y;
    Result(2, 3) = Translation.Z;
    return Result;
}

matrix4 ScaleMatrix(vector3 Scale)
{
    matrix4 Result = IdentityM4();
    Result(0, 0) = Scale.X;
    Result(1, 1) = Scale.Y;
    Result(2, 2) = Scale.Z;
    return Result;
}

matrix4 ChangeOfBasis(vector3 NewXAxis, vector3 NewYAxis, vector3 NewZAxis)
{
    matrix4 Result = IdentityM4();
    Result(0, 0) = NewXAxis.X;
    Result(1, 0) = NewXAxis.Y;
    Result(2, 0) = NewXAxis.Z;

    Result(0, 1) = NewYAxis.X;
    Result(1, 1) = NewYAxis.Y;
    Result(2, 1) = NewYAxis.Z;

    Result(0, 2) = NewZAxis.X;
    Result(1, 2) = NewZAxis.Y;
    Result(2, 2) = NewZAxis.Z;
    return Result;
}

matrix4 OrthographicProjection(f32 Left, f32 Right, f32 Top, f32 Bot, f32 Near, f32 Far)
{
    return Matrix(
            2.0f / (Right - Left), 0.0f, 0.0f, -(Left + Right) / (Right - Left),
            0.0f, 2.0f / (Top - Bot), 0.0f, -(Bot + Top) / (Top - Bot),
            0.0f, 0.0f, 2.0f / (Near - Far), -(Near + Far) / (Near - Far),
            0.0f, 0.0f, 0.0f, 1.0f);

}

matrix4 Frustum(f32 Left, f32 Right, f32 Top, f32 Bot, f32 Near, f32 Far)
{
    return Matrix(( 2.0f * Near) / (Right - Left), 0.0f, (Right + Left) / (Right - Left), 0.0f,
                    0.0f, (2.0f * Near) / (Top - Bot), (Top + Bot) / (Top - Bot), 0.0f,
                    0.0f, 0.0f, -(Far + Near) / (Far - Near), -(2.0f * Far * Near) / (Far - Near),
                    0.0f, 0.0f, -1.0f, 0.0f);
}

matrix4 LookAt(vector3 Eye, vector3 Target, vector3 WorldUp)
{
    vector3 Forward = Normalize(Target - Eye);
    vector3 Right = Normalize(Cross(Forward, WorldUp));
    vector3 Up = Cross(Right, Forward);

    matrix4 BaseChange = ChangeOfBasis(Right, Up, -Forward); //Note: Camera coordinate system z-axis needs to be flipped due to OpenGL
    matrix4 InverseBaseChange = Transpose(BaseChange); // Note: inverse == transpose because matrix is orthonormal.
    matrix4 InverseTranslation = TranslationMatrix(-Eye);

    return InverseBaseChange * InverseTranslation;
}

matrix4 PerspectiveProjection(f32 RadiansFOV, f32 Aspect, f32 Near, f32 Far)
{
    float TanHalfAngle = tanf(RadiansFOV / 2.0f);
    float Right = TanHalfAngle * Near;
    float Left = -Right;
    float Top = TanHalfAngle * Near / Aspect;
    float Bot = -Top;
    return Frustum(Left, Right, Top, Bot, Near, Far);
}