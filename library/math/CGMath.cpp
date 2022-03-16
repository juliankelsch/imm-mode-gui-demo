#include "CGMath.h"
#include <cmath>

f32 Lerp(f32 A, f32 B, f32 K)
{
    return (1.0f - K) * A + K * B;
}


f32 Bilerp(f32 A0, f32 A1, f32 B0, f32 B1, f32 T0, f32 T1)
{
    f32 A = Lerp(A0, A1, T0);
    f32 B = Lerp(B0, B1, T0);
    return Lerp(A, B, T1);
}

f32 Smoothstep(f32 Value, f32 Edge0, f32 Edge1)
{
    Value = Clamp((Value - Edge0) / (Edge1 - Edge0), 0.0f, 1.0f);
    return Value * Value * (3.0f - 2.0f * Value);
}

f32 Smoothstep(f32 Value)
{
    return Value * Value * (3.0f - 2.0f * Value);
}
f32 Smootherstep(f32 Value)
{
    return Value * Value * Value * (Value * (Value * 6.0f - 15.0f) + 10.0f);
}

f32 Smootherstep(f32 Value, f32 Edge0, f32 Edge1)
{
    Value = Clamp((Value - Edge0) / (Edge1 - Edge0), 0.0f, 1.0f);
    return Value * Value * Value * (Value * ( Value * 6.0f - 15.0f) + 10.0f);
}

f32 Sin(f32 Radians)
{
    return sinf(Radians);
}

f32 Cos(f32 Radians)
{
    return cosf(Radians);
}

f32 Sqrt(f32 Value)
{
    return sqrtf(Value);
}

f32 Max(f32 A, f32 B)
{
    return A < B ? B : A;
}

f32 Min(f32 A, f32 B)
{
   return A < B ? A : B;
}

f32 Deg2Rad(f32 Degrees)
{
    return (Degrees / 180.0f) * math::PI;
}

f32 Random01()
{
    return (f32)rand() / RAND_MAX;
}

f32 Rad2Deg(f32 Radians)
{
    return (Radians / math::PI) * 180.0f;
}

f32 SplineInterpolation(f32 T, matrix4 InterpolationMatrix, vector4 Geometry)
{
    vector4 MonomialVector = {T * T * T, T * T, T, 1};
    vector4 Coefficients = InterpolationMatrix * Geometry;
    return Dot(MonomialVector, Coefficients);
}

f32 SplineInterpolation(f32 T, matrix4 InterpolationMatrix, f32 G0, f32 G1, f32 G2, f32 G3)
{
    return SplineInterpolation(T, InterpolationMatrix, V(G0, G1, G2, G3));
}

f32 CatmullRomInterpolation(f32 T, f32 G0, f32 G1, f32 G2, f32 G3)
{
    return
          (-0.5f * G0 + 1.5f * G1 - 1.5f * G2 + 0.5f * G3) * (T * T * T)
        + ( 1.0f * G0 - 2.5f * G1 + 2.0f * G2 - 0.5f * G3) * (    T * T)
        + (-0.5f * G0             + 0.5f * G2            ) * (        T)
        + (             1.0f * G1);

}

matrix4 CatmullRom()
{
    return Matrix(-0.5f, 1.5f, -1.5f, 0.5f,
                  1.0f, -2.5f, 2.0f, -0.5f,
                  -0.5f, 0.0f, 0.5f, 0.0f,
                  0.0f, 1.0f, 0.0f, 0.0f);
}

matrix4 Bezier()
{
    return Matrix(-1.0f, 3.0f, -3.0f, 1.0f,
            3.0f, -6.0f, 3.0f, 0.0f,
            -3.0f, 3.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f, 0.0f);
}

matrix4 BSpline()
{
    return Matrix(-1.0f / 6.0f, 0.5f, -0.5f, 1.0f / 6.0,
            0.5f, -1.0f, 0.5f, 0.0f,
            -0.5f, 0.0f, 0.5f, 0.0f,
            1.0f / 6.0f, 2.0f / 3.0f, 1.0f / 6.0f, 0.0f);
}

bool PointInsideCircle(vector2 Point, vector2 CircleCenter, f32 CircleRadius)
{
    vector2 Between = CircleCenter - Point;
    return Dot(Between, Between) < CircleRadius * CircleRadius;
}

f32 Clamp(f32 Value, f32 Low, f32 High)
{
    return Value < Low ? Low : (Value > High) ? High : Value;
}

f32 Map(f32 Value, f32 FromLow, f32 FromHigh, f32 ToLow, f32 ToHigh)
{
    return Lerp(ToLow, ToHigh, (Value - FromLow) / (FromHigh - FromLow));
}


f32 FastInverseSquareRoot(f32 Value)
{
    f32 X2 = Value * 0.5f;
    f32 Y = Value;
    u64 I = *(u64*)&Y;
    I = 0x5f3759df - (I >> 1);
    Y = *(f32*)&I;
    Y = Y * (1.5f - (X2 * Y * Y)); // Note: 1 Newton Iteration
    return Y;
}

f32 SphereSDF(vector3 Point, vector3 SphereCenter, f32 SphereRadius)
{
    return Mag(Point - SphereCenter) - SphereRadius;
}

bool SolveQuadraticEquation(f32 A, f32 B, f32 C, f32* Solution0, f32* Solution1)
{
    f32 RootTerm = B * B - 4.0f * A * C;
    if(RootTerm < 0.0f)
        return false;
    RootTerm = Sqrt(RootTerm);
    *Solution0 = (-B + RootTerm) / (2.0f * A);
    *Solution1 = (-B - RootTerm) / (2.0f * A);
    return true;
}

f32 EaseInCirc(f32 Value)
{
    return 1.0f - Sqrt(1.0f - Value * Value);
}

f32 EaseOutCirc(f32 Value)
{
    return Sqrt(1.0f - (Value - 1.0f) * (Value - 1.0f));
}

f32 EaseInOutCirc(f32 Value)
{
    return Value < 0.5f
        ? (1.0f - Sqrt(1.0f - 4.0f * Value * Value)) / 2.0f
        : (Sqrt(1.0f - powf(-2.0f * Value + 2.0f, 2.0f)) + 1.0f) / 2.0f;
}

f32 EaseInSine(f32 Value)
{
    return 1.0f - Cos(Value * math::HALF_PI);
}

f32 EaseOutSine(f32 Value)
{
    return Sin(Value * math::HALF_PI);
}

f32 EaseInOutSine(f32 Value)
{
    return -(Cos(math::PI * Value) - 1.0f) / 2.0f;
}

f32 EaseInQuad(f32 Value)
{
    return Value * Value;
}
f32 EaseOutQuad(f32 Value)
{
    return 1.0f - (1.0f - Value) * (1.0f - Value);
}

f32 EaseInOutQuad(f32 Value)
{
    return Value < 0.5f ? 2.0f * Value * Value : 1.0f - powf(-2.0f * Value + 2.0f, 2.0f) / 2.0f;
}

f32 EaseInCubic(f32 Value)
{
    return Value * Value * Value;
}

f32 EaseOutCubic(f32 Value)
{
    return 1.0f - (1.0f - Value) * (1.0f - Value) * (1.0f - Value);

}

f32 EaseInOutCubic(f32 Value)
{
    return Value < 0.5f ? 4.0f * Value * Value * Value : 1.0f - powf(-2.0f * Value + 2.0f, 3.0f) / 2.0f;
}

f32 EaseInQuart(f32 Value)
{
    return powf(Value, 4.0f);
}
f32 EaseOutQuart(f32 Value)
{
    return 1.0f - powf(1.0f - Value, 4.0f);
}

f32 EaseInOutQuart(f32 Value)
{
    return Value < 0.5f ? 8.0f * powf(Value, 4.0f) : 1.0f - powf(-2.0f * Value + 2.0f, 4.0f) / 2.0f;
}

f32 EaseInQuint(f32 Value)
{
    return powf(Value, 5.0f);
}
f32 EaseOutQuint(f32 Value)
{
    return 1.0f - powf(1.0f - Value, 5.0f);
}

f32 EaseInOutQuint(f32 Value)
{
    return Value < 0.5f ? 16.0f * powf(Value, 5.0f) : 1.0f - powf(-2.0f * Value + 2.0f, 5.0f) / 2.0f;
}

f32 EaseInBack(f32 Value)
{
    const f32 C1 = 1.70158;
    const f32 C3 = C1 + 1.0f;

    return C3 * Value * Value * Value - C1 * Value * Value;
}
f32 EaseOutBack(f32 Value)
{
    const f32 C1 = 1.70158;
    const f32 C3 = C1 + 1.0f;

    return 1.0f + C3 * powf(Value - 1.0f, 3.0f) + C1 * powf(Value - 1.0f, 2.0f);
}

f32 EaseInOutBack(f32 Value)
{
    const f32 C1 = 1.70158;
    const f32 C2 = C1 + 1.525f;

    return Value < 0.5f
        ? (powf(2.0f * Value, 2.0f) * ((C2 + 1.0f) * 2.0f * Value - C2)) / 2.0f
        : (powf(2.0f * Value - 2.0f, 2.0f) * ((C2 + 1.0f) * (Value * 2.0f - 2.0f) + C2) + 2.0f) / 2.0f;
}

f32 EaseInElastic(f32 Value)
{
    const f32 C4 = math::TWO_PI / 3.0f;

    return Value == 0.0f
        ? 0.0f
        : Value == 1.0f
        ? 1.0f
        : -powf(2.0f, 10.0f * Value - 10.0f) * Sin((Value * 10.0f - 10.75) * C4);
}

f32 EaseOutElastic(f32 Value)
{
    const f32 C4 =  math::TWO_PI / 3.0f;
    return Value == 0.0f
           ? 0.0f
           : Value == 1.0f
           ? 1.0f
           : powf(2.0f, -10.0f * Value) * Sin((Value * 10.0f - 0.75) * C4) + 1.0f;
}

f32 EaseInOutElastic(f32 Value)
{
    const f32 C5 = math::TWO_PI / 4.5f;

    return Value == 0.0f
        ? 0.0f
        : Value == 1.0f
        ? 1.0f
        : Value < 0.5f
        ? -(powf(2.0f, 20.0f * Value - 10.0f) * Sin((20.0f * Value - 11.125f) * C5)) / 2.0f
        : (powf(2.0f, -20.0f * Value + 10.0f) * Sin((20.0f * Value - 11.125) * C5)) / 2.0f + 1.0f;

}

f32 EaseInBounce(f32 Value)
{
    return 1.0f - EaseOutBounce(1.0f - Value);
}

f32 EaseOutBounce(f32 Value)
{
    const f32 N1 = 7.5625f;
    const f32 D1 = 2.75f;

    if(Value < 1.0f / D1)
    {
        return N1 * Value * Value;
    }
    else if(Value < 2.0f / D1)
    {
        return N1 * (Value -= 1.5f / D1) * Value + 0.75f;
    }
    else if(Value < 2.5f / D1)
    {
        return N1 * (Value -= 2.25 / D1) * Value + 0.9375f;
    }
    else
    {
        return N1 * (Value -= 2.625f / D1) * Value + 0.984375f;
    }
}

f32 EaseInOutBounce(f32 Value)
{
    return Value < 0.5f
        ? (1.0f - EaseOutBounce(1.0f - 2.0f * Value)) / 2.0f
        : (1.0f + EaseOutBounce(2.0f * Value - 1.0f)) / 2.0f;
}