#include "Application.h"

#include "game/Game.h"
#include "platform/Platform.h"
#include "DebugLog.h"
#include "gui/GUI.h"
#include "Utility.h"
#include "stdio.h"
#include "opengl/OpenGL_ImmediateRenderer.h"
#include "projects/noise/NoiseProject.h"
#include "projects/snake/SnakeGame.h"
#include "Animations.h"

enum project : u32
{
    ProjectGame = 0,
    ProjectSnake,
    ProjectNoise,
    ProjectCount
};

enum app_mode : u32
{
    MainMenu,
    RunProject,
};

struct application_state
{
    bool ShowStats;
    input Input;
    app_mode Mode;
    project ActiveProject;

    GUI::font* MenuFont;
};

static application_state* AppState = nullptr;

static void Initialize()
{
    AppState->Mode = app_mode::RunProject;
    AppState->ActiveProject = ProjectNoise;

    CreateImmediateModeContext();
    GUI::CreateContext();
    GUI::SetupRenderer();

    AppState->MenuFont = GUI::LoadFont("../assets/fonts/Lato-Bold.ttf", 30.0f);
}

static void CycleThroughProjects()
{
    AppState->ActiveProject = static_cast<project>((AppState->ActiveProject + 1) % ProjectCount);
}

static void ToggleStats()
{
    AppState->ShowStats = !AppState->ShowStats;
}

static void HandleInput(input* Input)
{
    if(Input->IsPressed(KeyF10))
    {
        if(AppState->Mode == app_mode::MainMenu)
        {
            AppState->Mode = app_mode::RunProject;
        }
        else if(AppState->Mode == app_mode::RunProject)
        {
            AppState->Mode = app_mode::MainMenu;
        }
    }
    if(Input->IsPressed(KeyEsc))
        PlatformExit();
    if(Input->IsPressed(KeyF11))
        PlatformToggleFullscreen();
    //if(Input->IsPressed(KeyTab) && AppState->Mode == app_mode::RunProject)
        //CycleThroughProjects();
    if(Input->IsPressed(KeyF1))
        ToggleStats();
}

static void UpdateAndRender()
{
    input* Input = &AppState->Input;
    if(AppState->Mode == app_mode::RunProject)
    {
        switch(AppState->ActiveProject)
        {
            case ProjectGame:
                GameUpdateAndRender(&AppState->Input);
                break;
            case ProjectNoise:
                Noise::UpdateAndRender(&AppState->Input);
                break;
            case ProjectSnake:
                Snake::UpdateAndRender(&AppState->Input);
                break;
            default: break;
        }
    }
    else if(AppState->Mode == app_mode::MainMenu)
    {
        Clear(0.0f, 0.0f, 0.0f, 1.0f);
        GUI::SetFont(AppState->MenuFont);
        GUI::TextCentered(Input->Screen / 2.0f + V(0.0f, -100.0f), "MAIN MENU");
        color32 MenuItemColor = Color(0.6f, 0.6f, 0.6f, 1.0f);
        f32 Offset = 35.0f;
        GUI::TextCentered(Input->Screen / 2.0f + V(0.0f, 0.0f), "Resume", MenuItemColor);
        GUI::TextCentered(Input->Screen / 2.0f + V(0.0f, Offset), "Exit", MenuItemColor);
    }


    if(AppState->ShowStats)
    {
        GUI::SetDefaultFont();
        f32 LineHeight = GUI::GetCurrentFontSize() * 1.0f;
        u32 LineCount = 2;
        f32 Offset = 10.0f;

        GUI::Rect(V(0.0f, 0.0f), V(200.0f, Offset + LineCount * LineHeight), Color(0.0f, 0.0f, 0.0f, 0.6f));

        StringF(TextFPS, "%s %4dFPS", (DEBUG ? "DEBUG" : "RELEASE"), (u32)Input->Time.FramesPerSecond);
        GUI::Text(V(0.0f, 0.0f), TextFPS, Color(1.0f, 0.6f, 0.2f, 1.0f));
        char* ProjectName;
        switch (AppState->ActiveProject)
        {
            case ProjectNoise: ProjectName = "Noise Generator"; break;
            case ProjectGame: ProjectName = "Game"; break;
            case ProjectSnake: ProjectName = "Snake"; break;
        }
        GUI::Text(V(0.0f, LineHeight), ProjectName, Color(1.0f, 0.6f, 0.2f, 1.0f));
    }
}




void ApplicationUpdateAndRender(input_form* InputForm)
{
    if(!AppState)
    {
        AppState = (application_state*)PlatformAllocateMemoryZeroInitialized(sizeof(application_state));
        if(!AppState)
        {
            LogWarn("Platform could not allocate the memory for a valid application_state! Memory needed: %d bytes\n", sizeof(application_state));
            return;
        }

        Initialize();
    }

    InputUpdate(&AppState->Input, InputForm);
    input* Input = &AppState->Input;
    HandleInput(Input);

    GUI::StartFrame(Input);

    UpdateAndRender();

    GUI::EndFrame();
    GUI::Render(Input->FramebufferWidth, Input->FramebufferHeight, GUI::GetRenderList());
}

void ApplicationShutdown()
{
    GameShutdown();
    Noise::Shutdown();
    Snake::Shutdown();
    GUI::DeleteFont(AppState->MenuFont);
    GUI::DestroyContext();
    GUI::ShutdownRenderer();
    DestroyImmediateModeContext();
}
