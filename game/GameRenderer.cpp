#include "GameRenderer.h"
#include "opengl/OpenGL_Utility.h"
#include "glad/glad.h"
#include "Utility.h"
#include "opengl/OpenGL_ImmediateRenderer.h"

void InitializeRenderer(game_state* GameState)
{
    render_state* RenderState = &GameState->RenderState;
    RenderState->TerrainVertexShader = "../assets/glsl/color.vert";
    RenderState->TerrainFragmentShader = "../assets/glsl/color.frag";
    RenderState->TerrainProgram = CreateVertexFragmentShader(
            RenderState->TerrainVertexShader,
            RenderState->TerrainFragmentShader
    );

    glGenVertexArrays(1, &RenderState->TerrainVertexArray);
    glBindVertexArray(RenderState->TerrainVertexArray);

    vector3 Vertices[] = {
            {0.0f, 1.0f, -4.0f},
            {1.0f, 1.0f, -4.0f},
            {0.0f, 0.0f, -4.0f},
            {1.0f, 0.0f, -4.0f}
    };
    u32 TerrainVertexBuffer;
    glGenBuffers(1, &TerrainVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, TerrainVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

    u32 Indices[] = {
            0, 2, 1,
            1, 2, 3
    };
    RenderState->TerrainNumIndices = ARRAY_COUNT(Indices);
    u32 TerrainIndexBuffer;
    glGenBuffers(1, &TerrainIndexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, TerrainIndexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glDeleteBuffers(1, &TerrainVertexBuffer);

    ///////////////////////////
    RenderState->IsInitialized = true;
}

void RendererShutdown(render_state* RenderState)
{
    glDeleteProgram(RenderState->TerrainProgram);
    glDeleteVertexArrays(1, &RenderState->TerrainVertexArray);

    RenderState->TerrainProgram = 0;
    RenderState->TerrainVertexArray = 0;
}


void RenderGame(u32 FramebufferWidth, u32 FramebufferHeight, game_state* GameState)
{
    render_state* RenderState = &GameState->RenderState;
    if(!RenderState->IsInitialized)
    {
        InitializeRenderer(GameState);
    }

    glViewport(0, 0, FramebufferWidth, FramebufferHeight);
    glClearColor(0.04f, 0.04f, 0.04f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    f32 RadiansFOV = Deg2Rad(70.0f);
    f32 Aspect = FramebufferWidth / (f32)FramebufferHeight;
    f32 Near = 0.1f;
    f32 Far = 100.0f;

    vector3 Eye = GameState->CameraPos;
    vector3 Target = GameState->CameraTarget;
    vector3 WorldUp = V(0.0f, 1.0f, 0.0f);

    matrix4 ProjectionMatrix = PerspectiveProjection(RadiansFOV, Aspect, Near, Far);
    matrix4 ViewMatrix = LookAt(Eye, Target, WorldUp);

    /**

    glUseProgram(RenderState->TerrainProgram);
    SetMat4(RenderState->TerrainProgram, "Projection", ProjectionMatrix);
    SetMat4(RenderState->TerrainProgram, "View", ViewMatrix);

    glBindVertexArray(RenderState->TerrainVertexArray);
    glDrawElements(GL_TRIANGLES, RenderState->TerrainNumIndices, GL_UNSIGNED_INT, 0);
     */

    SetProjection(ProjectionMatrix);
    SetView(ViewMatrix);
    BindTexture(0);

    BeginPrimitive(Points);
    f32 Step = (2.0f * math::PI) / 30;
    f32 Radius = 0.8f;
    for (f32 Theta = 0.0f; Theta < math::PI; Theta += Step)
    {
        for (f32 Phi = 0.0f; Phi < (2.0f * math::PI); Phi += Step)
        {
            float X = Radius * Sin(Theta) * Cos(Phi);
            float Y = Radius * Sin(Theta) * Sin(Phi);
            float Z = Radius * Cos(Theta);
            SetColor(1.0f, Theta / math::PI, Phi / (2.0f * math::PI));
            AddVertex(X, Y, Z);
        }
    }
    EndPrimitive();

    SetColor(0.1f, 0.4f, 0.2f);
    f32 HalfSide = Radius;
    BeginPrimitive(Lines);
    AddVertex(HalfSide, HalfSide, HalfSide);
    AddVertex(-HalfSide, HalfSide, HalfSide);

    AddVertex(HalfSide, HalfSide, HalfSide);
    AddVertex(HalfSide, -HalfSide, HalfSide);

    AddVertex(-HalfSide, -HalfSide, HalfSide);
    AddVertex(HalfSide, -HalfSide, HalfSide);

    AddVertex(-HalfSide, -HalfSide, HalfSide);
    AddVertex(-HalfSide, HalfSide, HalfSide);

    AddVertex(HalfSide, HalfSide, -HalfSide);
    AddVertex(-HalfSide, HalfSide, -HalfSide);

    AddVertex(HalfSide, HalfSide, -HalfSide);
    AddVertex(HalfSide, -HalfSide, -HalfSide);

    AddVertex(-HalfSide, -HalfSide, -HalfSide);
    AddVertex(HalfSide, -HalfSide, -HalfSide);

    AddVertex(-HalfSide, -HalfSide, -HalfSide);
    AddVertex(-HalfSide, HalfSide, -HalfSide);

    AddVertex(HalfSide, HalfSide, HalfSide);
    AddVertex(HalfSide, HalfSide, -HalfSide);

    AddVertex(-HalfSide, HalfSide, HalfSide);
    AddVertex(-HalfSide, HalfSide, -HalfSide);

    AddVertex(HalfSide, -HalfSide, HalfSide);
    AddVertex(HalfSide, -HalfSide, -HalfSide);

    AddVertex(-HalfSide, -HalfSide, HalfSide);
    AddVertex(-HalfSide, -HalfSide, -HalfSide);

    EndPrimitive();

    BeginPrimitive(Lines);
    SetColor(1.0f, 0.0f, 0.0f);
    AddVertex(1.0f, 0.0f, 0.0f);
    AddVertex(0.0f, 0.0f, 0.0f);

    SetColor(0.0f, 1.0f, 0.0f);
    AddVertex(0.0f, 1.0f, 0.0f);
    AddVertex(0.0f, 0.0f, 0.0f);

    SetColor(0.0f, 0.0f, 1.0f);
    AddVertex(0.0f, 0.0f, 1.0f);
    AddVertex(0.0f, 0.0f, 0.0f);
    EndPrimitive();

}