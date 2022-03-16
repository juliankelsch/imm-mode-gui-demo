#ifndef PROCEDURALWORLD_COLOR_H
#define PROCEDURALWORLD_COLOR_H

#include "Types.h"

struct color32
{
    u8 R, G, B, A;
};

color32 Color(u32 HexCode);
color32 Color(f32 R, f32 G, f32 B, f32 A = 1.0f);

#endif //PROCEDURALWORLD_COLOR_H
