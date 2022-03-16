#include "RenderList2D.h"

void render_list_2d::Clear()
{
    VertexList.Clear();
    IndexList.Clear();
    DrawCalls.Clear();
}

static void AddDrawCall(render_list_2d* RenderList, u32 Start, u32 End, u32 TextureHandle, batch_mode Mode)
{
    draw_call NewDrawCall;

    NewDrawCall.BatchMode = Mode;
    NewDrawCall.Start = Start;
    NewDrawCall.End = End;
    NewDrawCall.TextureHandle = TextureHandle;

    RenderList->DrawCalls.Append(NewDrawCall);
}


void render_list_2d::AddMesh(vertex_2* Vertices, u32 VertexCount, u32* Indices, u32 IndexCount, u32 TextureID, bool Bitmap)
{
    if(DrawCalls.IsEmpty())
    {
        if(TextureID)
        {
            AddDrawCall(this, 0, 0, TextureID, Bitmap ? BatchModeBitmap : BatchModeImage);
        }
        else
        {
            AddDrawCall(this, 0, 0, TextureID, BatchModeColor);
        }
    }

    if(DrawCalls.LastElement().TextureHandle != TextureID)
    {
        DrawCalls.LastElement().End = IndexList.Length() - 1;
        batch_mode BatchMode;
        if(TextureID)
        {
            BatchMode = Bitmap ? BatchModeBitmap : BatchModeImage;
        }
        else
        {
            BatchMode = BatchModeColor;
        }
        AddDrawCall(this, IndexList.Length(), IndexList.Length(), TextureID, BatchMode);
    }

    DrawCalls.LastElement().End += IndexCount;

    u32 IndexOffset = VertexList.Length();

    for (u32 VertexIndex = 0; VertexIndex < VertexCount; ++VertexIndex)
        VertexList.Append(Vertices[VertexIndex]);

    for (u32 ElementIndex = 0; ElementIndex <  IndexCount; ++ElementIndex)
        IndexList.Append(Indices[ElementIndex] + IndexOffset);
}

void render_list_2d::AddRect(vector2 Min, vector2 Max, color32 Color)
{
    vertex_2 Vertices[] = {
            {{Min.X, Min.Y}, {0.0f, 0.0f}, Color},
            {{Max.X, Min.Y}, {0.0f, 0.0f}, Color},
            {{Min.X, Max.Y}, {0.0f, 0.0f}, Color},
            {{Max.X, Max.Y}, {0.0f, 0.0f}, Color},
    };
    u32 Indices[] = {
            0, 2, 1,
            3, 1, 2
    };
    AddMesh(Vertices, sizeof(Vertices) / sizeof(*Vertices), Indices, sizeof(Indices) / sizeof(*Indices), 0);
}