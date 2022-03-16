#include "SnakeGame.h"
#include "platform/Platform.h"
#include "DebugLog.h"
#include "gui/GUI.h"
#include <array>
#include <cmath>

constexpr u32 GRID_SIZE = 20;
constexpr f32 CELL_PIXEL_SIZE = 50.0f;
constexpr f32 TIMESTEP = 0.2f;

struct position
{
    s32 X, Y;
};

struct snake_game_state
{
    input* Input;
    position Snake;
    position Velocity;
    position Apple;
    std::array<position, GRID_SIZE * GRID_SIZE> Tail;
    u32 TailLength;
    f32 NextUpdateTime;
};

static snake_game_state* Game = nullptr;

static position RandomPosition()
{
    position Random = {rand() % static_cast<s32>(GRID_SIZE), rand() % static_cast<s32>(GRID_SIZE)};
    return Random;
}

static void ResetGame()
{
    Game->Snake = RandomPosition();
    Game->Apple = RandomPosition();
    Game->NextUpdateTime = Game->Input->ElapsedTime();
}

static position GetRandomEmptyPosition()
{

}

static void DrawCellAt(s32 X, s32 Y, color32 CellColor)
{
    vector2 GridCenter = Game->Input->Screen / 2.0f;
    vector2 CellsOrigin = GridCenter - V((GRID_SIZE / 2.0f) * CELL_PIXEL_SIZE, (GRID_SIZE / 2.0f) * CELL_PIXEL_SIZE);
    GUI::Circle(CellsOrigin + V(X * CELL_PIXEL_SIZE, Y * CELL_PIXEL_SIZE), CELL_PIXEL_SIZE / 2.0f - 2.0f, CellColor);
}

void Snake::UpdateAndRender(input *Input) {
    if(!Game)
    {
        Game = (snake_game_state*)PlatformAllocateMemoryZeroInitialized(sizeof(snake_game_state));
        if(!Game)
        {
            LogError("Could not allocate memory for noise project\n");
            return;
        }
        Game->Input = Input;
        ResetGame();
    }

    Game->Input = Input;
    if(Game->NextUpdateTime < Input->ElapsedTime())
    {

        if(Input->IsDown(KeyW))
        {
            Game->Velocity.X = 0;
            Game->Velocity.Y = -1;
        }
        else if(Input->IsDown(KeyS))
        {
            Game->Velocity.X = 0;
            Game->Velocity.Y = 1;
        }
        else if(Input->IsDown(KeyA))
        {
            Game->Velocity.X = -1;
            Game->Velocity.Y = 0;
        }
        else if(Input->IsDown(KeyD))
        {
            Game->Velocity.X = 1;
            Game->Velocity.Y = 0;
        }

        for (int I = Game->Tail.size() - 2; I >= 0; I--)
            Game->Tail[I + 1] = Game->Tail[I];
        Game->Tail[0] = Game->Snake;

        Game->Snake.X += Game->Velocity.X;
        Game->Snake.Y += Game->Velocity.Y;

        if(Game->Snake.X < 0)
            Game->Snake.X = GRID_SIZE - 1;
        if(Game->Snake.X >= GRID_SIZE)
            Game->Snake.X = Game->Snake.X % GRID_SIZE;

        if(Game->Snake.Y < 0)
            Game->Snake.Y = GRID_SIZE - 1;
        if(Game->Snake.Y >= GRID_SIZE)
            Game->Snake.Y = Game->Snake.Y % GRID_SIZE;

        if(Game->Apple.X == Game->Snake.X && Game->Apple.Y == Game->Snake.Y)
        {
            Game->Apple = RandomPosition();
            Game->TailLength++;
        }

        Game->NextUpdateTime += TIMESTEP;
    }

    color32 GridColor = Color(0.2f, 0.2f, 0.2f, 1.0f);
    color32 SnakeColor = Color(0.0f, 0.8f, 0.0f, 1.0f);
    color32 AppleColor = Color(0.8f, 0.0f, 0.0f, 1.0f);
    for (int I = 0; I < GRID_SIZE; ++I)
    {
        for (int J = 0; J < GRID_SIZE; ++J)
        {
            DrawCellAt(I, J, GridColor);
        }
    }

    DrawCellAt(Game->Snake.X, Game->Snake.Y, SnakeColor);
    DrawCellAt(Game->Apple.X, Game->Apple.Y, AppleColor);


    for (int I = 0; I < Game->TailLength; ++I)
    {
        DrawCellAt(Game->Tail[I].X, Game->Tail[I].Y, SnakeColor);
    }
}

void Snake::Shutdown()
{
    PlatformFreeMemory(Game);
    Game = nullptr;
}
