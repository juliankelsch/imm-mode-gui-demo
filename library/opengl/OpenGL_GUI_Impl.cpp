#include "gui/GUI.h"
#include "OpenGL_Utility.h"
#include "glad/glad.h"
#include "DebugLog.h"
#include "Matrix.h"

static u32 ProgramHandle;
static u32 VertexArrayHandle;
static u32 VertexBufferHandle;
static u32 ElementBufferHandle;

u32 GUI::CreateDeviceFontTexture(byte *Image8, u32 Width, u32 Height)
{
    return 0;
}

void GUI::SetupRenderer()
{
    ProgramHandle = CreateVertexFragmentShader("../assets/glsl/GUI.vert", "../assets/glsl/GUI.frag");
    Assert(ProgramHandle);

    glGenVertexArrays(1, &VertexArrayHandle);
    glBindVertexArray(VertexArrayHandle);

    glGenBuffers(1, &VertexBufferHandle);
    glBindBuffer(GL_ARRAY_BUFFER, VertexBufferHandle);

    glGenBuffers(1, &ElementBufferHandle);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementBufferHandle);
}

void GUI::ShutdownRenderer()
{
    glDeleteVertexArrays(1, &VertexArrayHandle);
    glDeleteBuffers(1, &VertexBufferHandle);
    glDeleteBuffers(1, &ElementBufferHandle);
    glDeleteProgram(ProgramHandle);

    VertexArrayHandle = 0;
    VertexBufferHandle = 0;
    ElementBufferHandle = 0;
    ProgramHandle = 0;
}

u32 GUI::CreateDeviceTexture(void* Data, u32 Channels, u32 Width, u32 Height)
{
    u32 TextureID;
    glGenTextures(1, &TextureID);
    glBindTexture(GL_TEXTURE_2D, TextureID);

    GLenum Format;
    switch(Channels)
    {
        case 4: Format = GL_RGBA; break;
        case 3: Format = GL_RGB; break;
        case 2: Format = GL_RG; break;
        case 1: Format = GL_RED; break;
        default: return 0;

    }

    glTexImage2D(GL_TEXTURE_2D, 0, Format, Width, Height, 0, Format, GL_UNSIGNED_BYTE, Data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    return TextureID;
}

void GUI::Render(u32 FramebufferWidth, u32 FramebufferHeight, render_list_2d* RenderList)
{
    glEnable(GL_CULL_FACE);
    //glEnable(GL_MULTISAMPLE);
    glCullFace(GL_BACK);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_TEXTURE_2D);

    glViewport(0, 0, FramebufferWidth, FramebufferHeight);

    glUseProgram(ProgramHandle);
    matrix4 ProjectionGUI = OrthographicProjection(0.0f, (f32)FramebufferWidth, 0.0f, (f32)FramebufferHeight, -1.0f, 1.0f);
    SetMat4(ProgramHandle, "ProjectionGUI", ProjectionGUI);
    glActiveTexture(GL_TEXTURE0);
    SetInt(ProgramHandle, "Image", 0);

    glBindVertexArray(VertexArrayHandle);

    glBindBuffer(GL_ARRAY_BUFFER, VertexBufferHandle);
    glBufferData(GL_ARRAY_BUFFER, RenderList->VertexList.Size(), RenderList->VertexList.GetData(), GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementBufferHandle);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, RenderList->IndexList.Size(), RenderList->IndexList.GetData(), GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(vertex_2), nullptr);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertex_2), (void*)(sizeof(vector2)));
    glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(vertex_2), (void*)(2 * sizeof(vector2)));

    for (int CallIndex = 0; CallIndex < RenderList->DrawCalls.Length(); ++CallIndex)
    {
        draw_call* DrawCall = &RenderList->DrawCalls[CallIndex];
        u32 Count = DrawCall->End - DrawCall->Start + 1;
        u32 Offset = RenderList->DrawCalls[CallIndex].Start;

        SetInt(ProgramHandle, "Mode", DrawCall->BatchMode);
        glBindTexture(GL_TEXTURE_2D, DrawCall->TextureHandle);
        glDrawElements(GL_TRIANGLES, Count, GL_UNSIGNED_INT, (void*)(Offset * sizeof(u32)));
    }
}
