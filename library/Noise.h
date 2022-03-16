#ifndef PROCEDURALWORLD_NOISE_H
#define PROCEDURALWORLD_NOISE_H

#include "Types.h"
#include "Vector.h"

#define MAX_NOISE_LENGTH 2000
#define MAX_NOISE_LENGTH_2D (MAX_NOISE_LENGTH * MAX_NOISE_LENGTH)

void PrecomputeValues1D(f32 Values[MAX_NOISE_LENGTH]);
void PrecomputeValues2D(f32 Values[MAX_NOISE_LENGTH_2D]);

void PrecomputeGradients1D(f32 Gradients[MAX_NOISE_LENGTH]);
void PrecomputeGradients2D(vector2 Gradients[MAX_NOISE_LENGTH_2D]);

f32 ValueNoise1D(f32 Values[MAX_NOISE_LENGTH], f32 X);
f32 ValueNoise2D(f32 Values[MAX_NOISE_LENGTH_2D], f32 X, f32 Y);

f32 PerlinNoise1D(f32     Gradients[MAX_NOISE_LENGTH], f32 X);
f32 PerlinNoise2D(vector2 Gradients[MAX_NOISE_LENGTH_2D], f32 X, f32 Y);

#endif //PROCEDURALWORLD_NOISE_H
