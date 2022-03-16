#ifndef PROCEDURALWORLD_GAME_H
#define PROCEDURALWORLD_GAME_H

#include "Types.h"
#include "Input.h"

struct render_state
{
    bool IsInitialized;

    char* TerrainVertexShader;
    char* TerrainFragmentShader;

    u32 TerrainProgram;
    u32 TerrainVertexArray;
    u32 TerrainNumIndices;
};

struct game_state {
    bool IsInitialized;
    input* Input;

    bool MoveCamera;
    f32 CameraTheta, CameraPhi;
    f32 CameraDegreesPerPixel;
    vector3 CameraPos;
    vector3 CameraTarget;

    static const u32 TerrainResolution = 100;
    static const u32 TerrainLength = 10.0f; // meters
    vector3 Terrain[TerrainResolution][TerrainResolution];

    render_state        RenderState;
};

void GameUpdateAndRender(input* Input);
void GameShutdown();

#endif //PROCEDURALWORLD_GAME_H
