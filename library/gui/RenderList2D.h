#ifndef PROCEDURALWORLD_RENDERLIST2D_H
#define PROCEDURALWORLD_RENDERLIST2D_H

#include "math/Vector.h"
#include "Color.h"
#include "DataStructures.h"

struct vertex_2
{
    vector2 Position;
    vector2 TexCoords;
    color32 Color;
};

enum batch_mode {BatchModeColor, BatchModeImage, BatchModeBitmap};

struct draw_call
{
    batch_mode BatchMode;
    u32 TextureHandle; //  vertex color is used when texture_id <= 0
    u32 Start, End; // start and end position in the index buffer
};

struct render_list_2d
{
    array_list<vertex_2> VertexList;
    array_list<u32> IndexList;
    array_list<draw_call> DrawCalls;

    void Clear();
    void AddMesh(vertex_2* Vertices, u32 VertexCount, u32* Indices, u32 IndexCount, u32 TextureID = 0, bool Bitmap = false);
    void AddRect(vector2 Min, vector2 Max, color32 Color = {255, 255, 255, 255});
};

#endif //PROCEDURALWORLD_RENDERLIST2D_H
