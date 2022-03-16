#include "OpenGL_ImmediateRenderer.h"
#include "math/Vector.h"
#include "glad/glad.h"
#include "DataStructures.h"
#include "OpenGL_Utility.h"

struct immediate_vertex
{
    vector3 Position;
    vector2 UV;
    u8 Red, Green, Blue, Alpha;
};

struct immediate_context
{
    primitive_type Primitive;
    u8 Red, Green, Blue, Alpha;
    vector2 UV;
    array_list<immediate_vertex> Vertices;

    matrix4 Projection, View, Model;

    // Note: Device objects
    u32 Program;
    u32 VertexArray;
    u32 VertexBuffer;

    u32 Texture;
    u32 DefaultTexture;
};

static immediate_context* Context;

void CreateImmediateModeContext()
{
    Context = (immediate_context*)PlatformAllocateMemoryZeroInitialized(sizeof(immediate_context));
    Context->Red = 255;
    Context->Green = 255;
    Context->Blue = 255;
    Context->Alpha = 255;

    Context->DefaultTexture = CreateTexture();
    u8 Data = 255;
    SetTextureData(Context->DefaultTexture, &Data, 1, 1, 1);
    Context->Texture = Context->DefaultTexture;

    Context->Projection = IdentityM4();
    Context->View = IdentityM4();
    Context->Model = IdentityM4();

    Context->Program = CreateVertexFragmentShader("../assets/glsl/immediate.vert", "../assets/glsl/immediate.frag");

    glGenVertexArrays(1, &Context->VertexArray);
    glBindVertexArray(Context->VertexArray);

    glGenBuffers(1, &Context->VertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, Context->VertexBuffer);
}

void DestroyImmediateModeContext()
{
    glDeleteProgram(Context->Program);
    glDeleteVertexArrays(1, &Context->VertexArray);
    glDeleteBuffers(1, &Context->VertexBuffer);

    Context->Program = 0;
    Context->VertexArray = 0;
    Context->VertexBuffer = 0;

    PlatformFreeMemory(Context);
    Context = nullptr;
}

void BeginPrimitive(primitive_type PrimitiveType)
{
    Context->Primitive = PrimitiveType;
}

static GLenum GetPrimitiveMode(primitive_type PrimitiveType)
{
    switch (PrimitiveType) {
        case Triangles: return GL_TRIANGLES;
        case Lines: return GL_LINES;
        case LineStrip: return GL_LINE_STRIP;
        case Points:
        default:
            return GL_POINTS;
    }
}

static void Render()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBindVertexArray(Context->VertexArray);
    glBindBuffer(GL_ARRAY_BUFFER, Context->VertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, Context->Vertices.Size(), Context->Vertices.GetData(), GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(immediate_vertex), 0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(immediate_vertex), (void*)(3 * sizeof(f32)));
    glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(immediate_vertex), (void*)(5 * sizeof(f32)));

    glUseProgram(Context->Program);
    matrix4 ImmediatePVM = Context->View * Context->Model;
    ImmediatePVM = Context->Projection * ImmediatePVM;
    SetMat4(Context->Program, "ImmediatePVM", ImmediatePVM);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, Context->Texture);
    SetInt(Context->Program, "BoundTexture", 0);

    glDrawArrays(GetPrimitiveMode(Context->Primitive), 0, Context->Vertices.Length());
}

void EndPrimitive()
{
    Render();
    Context->Vertices.Clear();
}

void SetProjection(const matrix4& ProjectionMatrix)
{
    Context->Projection = ProjectionMatrix;
}

void SetView(const matrix4& ViewMatrix)
{
    Context->View = ViewMatrix;
}

void SetModel(const matrix4& ModelMatrix)
{
    Context->Model = ModelMatrix;
}

void Clear(f32 Red, f32 Green, f32 Blue, f32 Alpha)
{
    glClearColor(Red, Green, Blue, Alpha);
    glClear(GL_COLOR_BUFFER_BIT);
}

void SetColor(f32 Red, f32 Green, f32 Blue)
{
    Context->Red = (u8)(255.0f * Red);
    Context->Green = (u8)(255.0f * Green);
    Context->Blue = (u8)(255.0f * Blue);
    Context->Alpha = 255;
}

void SetUV(f32 S, f32 T)
{
    Context->UV = V(S, T);
}

u32 CreateTexture()
{
    u32 Texture;
    glGenTextures(1, &Texture);
    glBindTexture(GL_TEXTURE_2D, Texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    return Texture;
}

void SetTextureData(u32 Texture, u8* Data, u32 Channels, u32 Width, u32 Height)
{
    if(Texture)
    {
        glBindTexture(GL_TEXTURE_2D, Texture);
        u32 Format;
        switch(Channels)
        {
            case 1:
            {
                Format = GL_RED;
                GLint SwizzleMask[] = {GL_ONE, GL_ONE, GL_ONE, GL_RED};
                glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, SwizzleMask);
                break;
            }
            case 2: Format = GL_RG; break;
            case 3: Format = GL_RGB; break;
            case 4: Format = GL_RGBA; break;
            default:
                return;
        }
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Width, Height, 0, Format, GL_UNSIGNED_BYTE, Data);
    }

}

void BindTexture(u32 Texture)
{
    Assert(Context);
    Context->Texture = Texture ? Texture : Context->DefaultTexture;
}

void AddVertex(f32 X, f32 Y, f32 Z)
{
    immediate_vertex NewVertex = {
            {X, Y, Z},
            Context->UV,
            Context->Red,
            Context->Green,
            Context->Blue,
            Context->Alpha
    };
    Context->Vertices.Append(NewVertex);
}
