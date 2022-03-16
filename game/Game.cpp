#include "Game.h"
#include "DebugLog.h"
#include "GameRenderer.h"
#include "platform/Platform.h"
#include "opengl/OpenGL_ImmediateRenderer.h"
#include "gui/GUI.h"
#include "Utility.h"


static game_state* GameState = nullptr;
static u8 Checkerboard[8 * 8];
static GUI::image CheckerboardImage;

void SetupGameState(game_state* GameState)
{

    for (int I = 0; I < 8; ++I) {
        for (int J = 0; J < 8; ++J) {
            Checkerboard[I * 8 + J] = ((I + J) % 2) == 0 ? 0 : 255;
        }
    }
    CheckerboardImage = GUI::LoadImage(Checkerboard, 1, 8, 8);

    //Note: The game state is always guaranteed to be 0-initialized by the platform layer.
    f32 Radius = 5.0f;
    GameState->CameraTheta = Clamp(GameState->CameraTheta, 0.0f + Deg2Rad(0.1f), math::PI - Deg2Rad(0.1f));
    GameState->CameraPos = V(
            Radius * Sin(GameState->CameraTheta) * Cos(GameState->CameraPhi),
            Radius * Cos(GameState->CameraTheta),
            Radius * Sin(GameState->CameraTheta) * Sin(GameState->CameraPhi)
    );
    GameState->CameraTarget = V(0.0f, 0.0f, 0.0f);
    GameState->CameraDegreesPerPixel = 0.1f;

    GameState->IsInitialized = true;
}

void ProcessInput(game_state* GameState)
{
    input* Input = GameState->Input;
    GameState->MoveCamera = Input->Mouse.ButtonInfos[MouseButtonLeft].State == Held;
}

void GameUpdateAndRender(input* Input)
{

    if(!GameState)
    {
        GameState = (game_state*)PlatformAllocateMemoryZeroInitialized(sizeof(game_state));
        if(!GameState)
        {
           LogWarn("Platform could not allocate the memory for a valid game_state! Memory needed: %d bytes\n", sizeof(game_state));
           return;
        }
        SetupGameState(GameState);
    }
    GameState->Input = Input;
    ProcessInput(GameState);


    if(GameState->MoveCamera)
    {
        GameState->CameraPhi += Deg2Rad(GameState->CameraDegreesPerPixel) * GameState->Input->Mouse.Motion.X;
        GameState->CameraTheta += Deg2Rad(GameState->CameraDegreesPerPixel) * -GameState->Input->Mouse.Motion.Y;
        GameState->CameraTheta = Clamp(GameState->CameraTheta, 0.0f + Deg2Rad(0.1f), math::PI - Deg2Rad(0.1f));

        f32 Radius = 5.0f;
        GameState->CameraPos = V(
                Radius * Sin(GameState->CameraTheta) * Cos(GameState->CameraPhi),
                Radius * Cos(GameState->CameraTheta),
                Radius * Sin(GameState->CameraTheta) * Sin(GameState->CameraPhi)
        );
    }

    RenderGame((u32)Input->FramebufferWidth, (u32)Input->FramebufferHeight, GameState);


}

void GameShutdown()
{
    if(GameState)
    {
        RendererShutdown(&GameState->RenderState);
    }
}
