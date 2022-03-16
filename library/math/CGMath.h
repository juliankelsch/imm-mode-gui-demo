#ifndef CG_FUN_CGMATH_H
#define CG_FUN_CGMATH_H

#include "Vector.h"
#include "Matrix.h"
#include "Quaternion.h"

struct math
{
    static constexpr f32 PI = 3.141592653f;
    static constexpr f32 HALF_PI = 1.57079632678f;
    static constexpr f32 TWO_PI = 6.28318530718f;
    static constexpr f32 TAU = 6.28318530718f;
    static constexpr f32 E = 2.718281828459f;
};



f32 Sin(f32 Radians);
f32 Cos(f32 Radians);
f32 Sqrt(f32 Value);
f32 Max(f32 A, f32 B);
f32 Min(f32 A, f32 B);
//Note: Based on rand(), Could be faster, include "Random.h" for or use <random>
f32 Random01();
f32 Deg2Rad(f32 Degrees);
f32 Rad2Deg(f32 Radians);
bool PointInsideCircle(vector2 Point, vector2 CircleCenter, f32 CircleRadius);
f32 Clamp(f32 Value, f32 Low, f32 High);
f32 Map(f32 Value, f32 FromLow, f32 FromHigh, f32 ToLow, f32 ToHigh);
// Note: Quake III Q_rsqrt https://en.wikipedia.org/wiki/Fast_inverse_square_root
f32 FastInverseSquareRoot(f32 Value);

//Note: K, T0, T1 are weights
f32 Lerp(f32 A, f32 B, f32 K);
f32 Bilerp(f32 A0, f32 A1, f32 B0, f32 B1, f32 T0, f32 T1);
f32 Smoothstep(f32 Value, f32 Edge0, f32 Edge1);
f32 Smootherstep(f32 Value, f32 Edge0, f32 Edge1);

f32 EaseInCirc(f32 Value);
f32 EaseOutCirc(f32 Value);
f32 EaseInOutCirc(f32 Value);

f32 EaseInSine(f32 Value);
f32 EaseOutSine(f32 Value);
f32 EaseInOutSine(f32 Value);

f32 EaseInQuad(f32 Value);
f32 EaseOutQuad(f32 Value);
f32 EaseInOutQuad(f32 Value);

f32 EaseInCubic(f32 Value);
f32 EaseOutCubic(f32 Value);
f32 EaseInOutCubic(f32 Value);

f32 EaseInQuart(f32 Value);
f32 EaseOutQuart(f32 Value);
f32 EaseInOutQuart(f32 Value);

f32 EaseInQuint(f32 Value);
f32 EaseOutQuint(f32 Value);
f32 EaseInOutQuint(f32 Value);

f32 EaseInBack(f32 Value);
f32 EaseOutBack(f32 Value);
f32 EaseInOutBack(f32 Value);

f32 EaseInElastic(f32 Value);
f32 EaseOutElastic(f32 Value);
f32 EaseInOutElastic(f32 Value);

f32 EaseInBounce(f32 Value);
f32 EaseOutBounce(f32 Value);
f32 EaseInOutBounce(f32 Value);

f32 Smoothstep(f32 Value);
f32 Smootherstep(f32 Value);

f32 SplineInterpolation(f32 T, matrix4 InterpolationMatrix, vector4 Geometry);
f32 SplineInterpolation(f32 T, matrix4 InterpolationMatrix, f32 G0, f32 G1, f32 G2, f32 G3);
f32 CatmullRomInterpolation(f32 T, f32 G0, f32 G1, f32 G2, f32 G3);
matrix4 CatmullRom();
matrix4 Bezier();
matrix4 BSpline();

//Note: Returns true if the equation is solvable. [AX^2 + BX + C = 0]
bool SolveQuadraticEquation(f32 A, f32 B, f32 C, f32* Solution0, f32* Solution1);

f32 SphereSDF(vector3 Point, vector3 SphereCenter, f32 SphereRadius);

#endif //CG_FUN_CGMATH_H
