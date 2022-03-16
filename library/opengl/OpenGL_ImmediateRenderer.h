#ifndef PROCEDURALWORLD_OPENGL_IMMEDIATERENDERER_H
#define PROCEDURALWORLD_OPENGL_IMMEDIATERENDERER_H

#include "Types.h"
#include "math/CGMath.h"

void CreateImmediateModeContext();
void DestroyImmediateModeContext();

enum primitive_type {Points, Triangles, Lines, LineStrip, Quads};
void BeginPrimitive(primitive_type PrimitiveType);
void EndPrimitive();

void SetProjection(const matrix4& ProjectionMatrix);
void SetView(const matrix4& ViewMatrix);
void SetModel(const matrix4& ModelMatrix);

u32 CreateTexture();
void SetTextureData(u32 Texture, u8* Data, u32 Channels, u32 Width, u32 Height);
void BindTexture(u32 Texture);

void Clear(f32 Red, f32 Green, f32 Blue, f32 Alpha);

void SetColor(f32 Red, f32 Green, f32 Blue);
void SetUV(f32 S, f32 T);
void AddVertex(f32 X, f32 Y, f32 Z);

#endif //PROCEDURALWORLD_OPENGL_IMMEDIATERENDERER_H
