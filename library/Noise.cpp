#include "Noise.h"
#include "CGMath.h"

void PrecomputeValues1D(f32 Values[MAX_NOISE_LENGTH])
{
    for (int I = 0; I < MAX_NOISE_LENGTH; ++I)
        Values[I] = Random01();
}

static f32 Interpolate(f32 A0, f32 A1, f32 W)
{
    //W = W * W *(3.0f - W * 2.0f); // Note: Cubic interpolation
    W = ((W * (W * 6.0f - 15.0f) + 10.0f) * W * W * W); // Note: Zero Second derivatives on boundaries
    return Lerp(A0, A1, W);
}

void PrecomputeValues2D(f32 Values[MAX_NOISE_LENGTH_2D])
{
    for (int I = 0; I < MAX_NOISE_LENGTH_2D; ++I)
        Values[I] = Random01();
}

void PrecomputeGradients1D(f32 Gradients[MAX_NOISE_LENGTH])
{
    for (int I = 0; I < MAX_NOISE_LENGTH; ++I)
        Gradients[I] = Random01() * 2.0f - 1.0f;

}

void PrecomputeGradients2D(vector2 Gradients[MAX_NOISE_LENGTH_2D])
{
    for (int I = 0; I < MAX_NOISE_LENGTH_2D; ++I)
    {
        f32 Angle = Random01() * math::TWO_PI;
        Gradients[I] = V(Cos(Angle), Sin(Angle));
    }
}

f32 ValueNoise1D(f32 Values[MAX_NOISE_LENGTH], f32 X)
{
    // Compute indices
    s32 X0 = (u32)X % MAX_NOISE_LENGTH;
    s32 X1 = (X0 + 1) % MAX_NOISE_LENGTH;

    f32 Weight = X - X0;
    return Interpolate(Values[X0], Values[X1], Weight);
}

/*
f32 PerlinNoise1D(vector2 Gradients[MAX_NOISE_LENGTH], f32 X)
{
    // Compute indices
    s32 I1 = (u32)X;
    s32 I0 = ((I1 - 1) < 0) ? (MAX_NOISE_LENGTH - 1) : (I1 - 1);
    s32 I2 = (I1 + 1) % MAX_NOISE_LENGTH;
    s32 I3 = (I1 + 2) % MAX_NOISE_LENGTH;

    f32 Weight = X - I1;
    return CatmullRomInterpolation(Weight, Values[I0], Values[I1], Values[I2], Values[I3]);
}
 */


f32 ValueNoise2D(f32 Values[MAX_NOISE_LENGTH_2D], f32 X, f32 Y)
{
    s32 X0 = (u32)X % MAX_NOISE_LENGTH;
    s32 X1 = (X0 + 1) % MAX_NOISE_LENGTH;
    s32 Y0 = (u32)Y % MAX_NOISE_LENGTH;
    s32 Y1 = (Y0 + 1) % MAX_NOISE_LENGTH;

    f32 S = X - X0;
    f32 T = Y - Y0;

    f32 A0 = Values[Y0 * MAX_NOISE_LENGTH + X0];
    f32 A1 = Values[Y0 * MAX_NOISE_LENGTH + X1];
    f32 A = Interpolate(A0, A1, S);

    f32 B0 = Values[Y1 * MAX_NOISE_LENGTH + X0];
    f32 B1 = Values[Y1 * MAX_NOISE_LENGTH + X1];
    f32 B = Interpolate(B0, B1, S);

    return Interpolate(A, B, T);
}

f32 PerlinNoise1D(f32 Gradients[MAX_NOISE_LENGTH], f32 X)
{
    // Compute indices
    s32 X0 = (u32)X % MAX_NOISE_LENGTH;
    s32 X1 = (X0 + 1) % MAX_NOISE_LENGTH;

    f32 Weight = X - X0;
    return Interpolate(Weight * Gradients[X0], -(1.0f - Weight) * Gradients[X1], Weight);
}

f32 PerlinNoise2D(vector2 Gradients[MAX_NOISE_LENGTH_2D], f32 X, f32 Y)
{
    s32 X0 = (u32)X % MAX_NOISE_LENGTH;
    s32 X1 = (X0 + 1) % MAX_NOISE_LENGTH;
    s32 Y0 = (u32)Y % MAX_NOISE_LENGTH;
    s32 Y1 = (Y0 + 1) % MAX_NOISE_LENGTH;

    f32 S = X - X0;
    f32 T = Y - Y0;

    vector2 P = V(X, Y);
    f32 A0 = Dot(P - V(X0, Y0), Gradients[Y0 * MAX_NOISE_LENGTH + X0]);
    f32 A1 = Dot(P - V(X1, Y0), Gradients[Y0 * MAX_NOISE_LENGTH + X1]);
    f32 A = Interpolate(A0, A1, S);

    f32 B0 = Dot(P - V(X0, Y1), Gradients[Y1 * MAX_NOISE_LENGTH + X0]);
    f32 B1 = Dot(P - V(X1, Y1), Gradients[Y1 * MAX_NOISE_LENGTH + X1]);
    f32 B = Interpolate(B0, B1, S);
    return Interpolate(A, B, T);
}
