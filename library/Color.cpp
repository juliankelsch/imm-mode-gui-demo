#include "Color.h"

color32 Color(u32 HexCode)
{
    // TODO: Implement
    return Color(1.0f, 1.0f, 1.0f, 1.0f);
}

color32 Color(f32 R, f32 G, f32 B, f32 A)
{
    color32 NewColor = {(u8)(R * 255.0f), (u8)(G * 255.0f), (u8)(B * 255.0f), (u8)(A * 255.0f)};
    return NewColor;
}
