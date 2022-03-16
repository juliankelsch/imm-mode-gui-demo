//
// Created by julia on 3/12/2021.
//
#include <functional>

#include "Animations.h"
#include "Noise.h"
#include "NoiseProject.h"
#include "platform/Platform.h"
#include "DebugLog.h"
#include "opengl/OpenGL_ImmediateRenderer.h"
#include "Color.h"
#include "gui/GUI.h"
#include <array>

struct point
{
    vector2 Position;
    color32 Color;
};

static constexpr u32 Width      = 500;
static constexpr u32 Height     = 500;
static constexpr u32 NumPoints  =  70;

struct noise_state
{
    color32 Pixels[Width][Height];
    point   Points[NumPoints];
    f32     RandomValues[MAX_NOISE_LENGTH];
    f32     RandomValues2D[MAX_NOISE_LENGTH_2D];
    f32     Gradients[MAX_NOISE_LENGTH];
    vector2 Gradients2D[MAX_NOISE_LENGTH_2D];

    u32 Octaves;
    f32 Offset;
    f32 Lacunarity;
    f32 Persistence;
    f32 SampleScale;

    bool    TopLeft, TopRight, BotLeft, BotRight;
    f32     CornerRadius;
    u32     CornerResolution;
    color32 BoxColor;
    f32     BoxWidth;

    u32 ImageHandle;
};

static noise_state* State = nullptr;

static void Initialize()
{
    State->ImageHandle = CreateTexture();
    for (int X = 0; X < Width; ++X)
    {
        for (int Y = 0; Y < Height; ++Y)
        {
            State->Pixels[X][Y].A = 255;
        }
    }

    PrecomputeValues1D(State->RandomValues);
    PrecomputeValues2D(State->RandomValues2D);
    PrecomputeGradients1D(State->Gradients);
    PrecomputeGradients2D(State->Gradients2D);

    for (int I = 0; I < NumPoints; ++I)
    {
        State->Points[I].Position.X = Random01();
        State->Points[I].Position.Y = Random01();
        State->Points[I].Color = Color(Random01(), Random01(), 0.0f, 1.0f);
    }
}

static f32 EuclideanDistanceSquared(vector2 A, vector2 B)
{
    return Dot(A-B, A-B);
}

static f32 ManhattanDistance(vector2 A, vector2 B)
{
    vector2 Delta = A - B;
    return abs(Delta.X) + abs(Delta.Y);

}

static void ClearImage()
{
    for (int X = 0; X < Width; ++X)
    {
        for (int Y = 0; Y < Height; ++Y)
        {
            State->Pixels[X][Y] = Color(0.0f, 0.0f, 0.0f, 0.0f);
        }
    }

}

static point FindClosestPoint(vector2 Position, point* Points, u32 NumPoints,
        std::function<f32(vector2 A, vector2 B)> DistanceFunction, f32* Distance)
{
    Assert(NumPoints);
    u32 ClosestIndex = 0;
    f32 ClosestDistance = DistanceFunction(Position, Points[ClosestIndex].Position);
    for (int I = 1; I < NumPoints; ++I)
    {
        f32 Dist = DistanceFunction(Position, Points[I].Position);
        if(Dist < ClosestDistance)
        {
            ClosestDistance = Dist;
            ClosestIndex = I;
        }
    }
    *Distance = ClosestDistance;
    return Points[ClosestIndex];
}

f32 Step(f32 Threshold, f32 Value)
{
    return Value < Threshold ? 0.0f : 1.0f;
}

static void Voronoi()
{
    for (int X = 0; X < Width; ++X)
    {
        for (int Y = 0; Y < Height; ++Y)
        {
            vector2 Position = {X / (Width - 1.0f), Y / (Height - 1.0f)};
            f32 Distance;
            FindClosestPoint(Position, State->Points, NumPoints, EuclideanDistanceSquared, &Distance);
            f32 Intensity = Distance * 20;
            State->Pixels[X][Y] = Color(Intensity, Intensity, 0.5f * Intensity, 1.0f);
        }
    }
}

static void Value1D()
{
    for (int X = 0; X < Width; ++X)
    {
        for (int Y = 0; Y < Height; ++Y)
        {
            vector2 Position = {X / (Width - 1.0f), Y / (Height - 1.0f)};
            f32 Intensity = ValueNoise1D(State->RandomValues, 100.0f * Position.X);
            State->Pixels[X][Y] = Color(Intensity, Intensity, Intensity, 1.0f);
        }
    }
}

static void Perlin1D()
{

    for (int X = 0; X < Width; ++X)
    {
        for (int Y = 0; Y < Height; ++Y)
        {
            vector2 Position = {X / (Width - 1.0f), Y / (Height - 1.0f)};
            u32 Octaves = 4;
            f32 Intensity = 0.0f;
            f32 Persistence = 0.3f;
            for (int I = 0; I < Octaves; ++I)
            {
                u32 Frequency = 1 << I;
                f32 Amplitude = pow(Persistence, I);
                Intensity += Amplitude * (PerlinNoise1D(State->Gradients, Frequency * Position.X) * 0.5f + 0.5f);
            }
            State->Pixels[X][Y] = Color(Intensity, Intensity, Intensity, 1.0f);
        }
    }
}

static void Value2D()
{
    for (int X = 0; X < Width; ++X)
    {
        for (int Y = 0; Y < Height; ++Y)
        {
            vector2 Position = {X / (Width - 1.0f), Y / (Height - 1.0f)};
            f32 Intensity = ValueNoise2D(State->RandomValues2D, 8.0f * Position.X, 8.0f * Position.Y);
            State->Pixels[X][Y] = Color(Intensity, Intensity, Intensity, 1.0f);
        }
    }
}

static void NormalizeImage()
{
    f32 MinI = 9999999.9f;
    f32 MaxI = -99999999.9f;
    for (int X = 0; X < Width; ++X)
    {
        for (int Y = 0; Y < Height; ++Y)
        {
            MinI = Min(MinI, State->Pixels[X][Y].R / 255.0f);
            MaxI = Max(MaxI, State->Pixels[X][Y].R/ 255.0f);
        }
    }

    for (int X = 0; X < Width; ++X)
    {
        for (int Y = 0; Y < Height; ++Y)
        {
            f32 Intensity = Map(State->Pixels[X][Y].R / 255.0f, MinI, MaxI, 0.0f, 1.0f);
            Intensity = Intensity * Intensity * Intensity;
            State->Pixels[X][Y] = Color(Intensity, Intensity, Intensity, 1.0f);
        }
    }
}

static void PerlinFractal2D(u32 Octaves, f32 Lacunarity, f32 Persistence, f32 SampleScale)
{
    for (int X = 0; X < Width; ++X)
    {
        for (int Y = 0; Y < Height; ++Y)
        {
            vector2 Position = {X / (Width - 1.0f), Y / (Height - 1.0f)};
            f32 Intensity = 0.0f;
            for (int I = 0; I < Octaves; ++I)
            {
                u32 Frequency = pow(Lacunarity, I);
                f32 Amplitude = pow(Persistence, I);
                Intensity += Amplitude * PerlinNoise2D(State->Gradients2D, State->Offset + SampleScale * Frequency * Position.X, State->Offset + SampleScale * Frequency * Position.Y);
            }
            Intensity = Intensity * 0.5f + 0.5f;
            State->Pixels[X][Y] = Color(Intensity, Intensity, Intensity, 1.0f);
        }
    }
}

static void Perlin2D()
{
    for (int X = 0; X < Width; ++X)
    {
        for (int Y = 0; Y < Height; ++Y)
        {
            vector2 Position = {X / (Width - 1.0f), Y / (Height - 1.0f)};
            u32 Octaves = 4;
            f32 Intensity = 0.0f;
            f32 Persistence = 0.3f;
            f32 Lacunarity = 3.3f;
            f32 SampleScale = 7.0f;
            for (int I = 0; I < Octaves; ++I)
            {
                u32 Frequency = pow(Lacunarity, I);
                f32 Amplitude = pow(Persistence, I);
                Intensity += Amplitude * PerlinNoise2D(State->Gradients2D, SampleScale * Frequency * Position.X, SampleScale * Frequency * Position.Y);
            }
            Intensity = Intensity * 0.5f + 0.5f;
            State->Pixels[X][Y] = Color(Intensity, Intensity, Intensity, 1.0f);
        }
    }
}

void Noise::UpdateAndRender(input *Input) {
    if(!State)
    {
        State = (noise_state*)PlatformAllocateMemoryZeroInitialized(sizeof(noise_state));
        if(!State)
        {
            LogError("Could not allocate memory for noise project\n");
            return;
        }
        Initialize();
    }

    GUI::SetDefaultFont();
    GUI::corner_mask CornerMask = GUI::ROUNDED_NONE;
    if(GUI::Checkbox("Top Left",  &State->TopLeft,  V(20.0f, 220.0f)))
        CornerMask = (GUI::corner_mask)(CornerMask | GUI::ROUNDED_TOP_LEFT);
    if(GUI::Checkbox("Top Right", &State->TopRight, V(120.0f, 220.0f)))
        CornerMask = (GUI::corner_mask)(CornerMask | GUI::ROUNDED_TOP_RIGHT);
    if(GUI::Checkbox("Bot Left",  &State->BotLeft,  V(20.0f, 240.0f)))
        CornerMask = (GUI::corner_mask)(CornerMask | GUI::ROUNDED_BOT_LEFT);
    if(GUI::Checkbox("Bot Right", &State->BotRight, V(120.0f, 240.0f)))
        CornerMask = (GUI::corner_mask)(CornerMask | GUI::ROUNDED_BOT_RIGHT);

    GUI::SliderColorRGBA("Box Color", &State->BoxColor, V(20.0f, 300.0f));
    GUI::SliderInt("Corner Resolution", &State->CornerResolution, 1, 10, V(20.0f, 260.0f));
    GUI::SliderFloat("Corner Radius", &State->CornerRadius, 2.0f, 100.0f, V(20.0f, 280.0f));
    GUI::SliderFloat("Box Width", &State->BoxWidth, 90.0f, 360.0f, V(20.0f, 400.0f));
    GUI::RoundedRect(V(20.0f, 20.0f), V(20.0f + State->BoxWidth, 20.0f + 180.0f),
            State->CornerRadius, CornerMask, State->BoxColor, State->CornerResolution);

    if(Input->IsPressed(KeyEnter))
    {
        ClearImage();
        SetTextureData(State->ImageHandle, (u8*)State->Pixels, 4, Width, Height);
    }

    if( GUI::SliderInt("Octaves", &State->Octaves, 1, 6, V(20.0f, 500.0f))
        | GUI::SliderFloat("Persistence", &State->Persistence, 0.0f, 1.0f, V(20.0f, 520.0f))
        | GUI::SliderFloat("Lacunarity", &State->Lacunarity, 1.0f, 10.0f, V(20.0f, 540.0f))
        | GUI::SliderFloat("SampleScale", &State->SampleScale, 1.0f, 10.0f, V(20.0f, 560.0f))
        | GUI::SliderFloat("Offset", &State->Offset, 0.0f, 100.0f, V(20.0f, 580.0f)))
    {
        PerlinFractal2D(State->Octaves, State->Lacunarity, State->Persistence, State->SampleScale);
        NormalizeImage();
        SetTextureData(State->ImageHandle, (u8*)State->Pixels, 4, Width, Height);
    }

    Clear(0.05f, 0.05f, 0.05f, 1.0f);
    SetProjection(OrthographicProjection(0.0f, Input->FramebufferWidth, 0.0f, Input->FramebufferHeight, -1.0f, 1.0f));
    SetView(IdentityM4());
    SetModel(IdentityM4());
    BindTexture(State->ImageHandle);

    vector2 Center = V(Input->FramebufferWidth, Input->FramebufferHeight) / 2.0f;
    vector2 HalfSize = {Width / 2.0f, Height / 2.0f};
    BeginPrimitive(Triangles);
    {
        SetUV(0.0f, 0.0f); AddVertex(Center.X - HalfSize.X, Center.Y - HalfSize.Y, 0.0f);
        SetUV(0.0f, 1.0f); AddVertex(Center.X - HalfSize.X, Center.Y + HalfSize.Y, 0.0f);
        SetUV(1.0f, 0.0f); AddVertex(Center.X + HalfSize.X, Center.Y - HalfSize.Y, 0.0f);

        SetUV(1.0f, 0.0f); AddVertex(Center.X + HalfSize.X, Center.Y - HalfSize.Y, 0.0f);
        SetUV(0.0f, 1.0f); AddVertex(Center.X - HalfSize.X, Center.Y + HalfSize.Y, 0.0f);
        SetUV(1.0f, 1.0f); AddVertex(Center.X + HalfSize.X, Center.Y + HalfSize.Y, 0.0f);
    }
    EndPrimitive();
}

void Noise::Shutdown()
{
    PlatformFreeMemory(State);
    State = nullptr;
}