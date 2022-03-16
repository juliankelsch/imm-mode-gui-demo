#ifndef PROCEDURALWORLD_INPUT_H
#define PROCEDURALWORLD_INPUT_H

#include "Types.h"
#include "math/CGMath.h"

enum key {
    KeyA, KeyB, KeyC, KeyD, KeyE, KeyF, KeyG, KeyH, KeyI, KeyJ, KeyK, KeyL, KeyM,
    KeyN, KeyO, KeyP, KeyQ, KeyR, KeyS, KeyT, KeyU, KeyV, KeyW, KeyX, KeyY, KeyZ,

    KeyF1, KeyF2, KeyF3, KeyF4, KeyF5, KeyF6, KeyF7, KeyF8, KeyF9, KeyF10, KeyF11, KeyF12,

    Key0, Key1, Key2, Key3, Key4, Key5, Key6, Key7, Key8, Key9,

    KeyTab, KeyEnter, KeyEsc, KeyBackspace,
    KeySpace,
    KeyCount,
};

enum mouse_button
{
    MouseButtonLeft,
    MouseButtonRight,
    MouseButtonMiddle,
    MouseButtonCount
};

// Note: This is to be filled out be the platform layer and given to the application/game layer
struct input_form
{
    s32 FramebufferWidth;
    s32 FramebufferHeight;
    f64 MouseX;
    f64 MouseY;
    f64 ScrollX;
    f64 ScrollY;
    f64 Time;

    bool MouseButtonsPressed[MouseButtonCount];
    bool KeysPressed[KeyCount];
};

enum button_state
{
    None,
    Pressed,    // Note: True in the first frame the key is pressed
    Released,   // Note: True in the first frame the key is released
    Held,       // Note: True in all the frames after Pressed and before Released
};

struct button_info
{
    button_state State;
};

struct keyboard_input
{
    button_info KeyInfos[KeyCount];
    /*
    // Note: Following boolean values are true when one or both of their Left/ Right versions are either Pressed or Held down
    bool CtrlDown;
    bool AltDown;
     */
};

struct mouse_input
{
    button_info ButtonInfos[MouseButtonCount];
    vector2 Position;
    vector2 Motion;
    f32 Scroll;
};

struct time_info
{
    f32                 ElapsedTime;
    f32                 DeltaTime;
    f32                 FramesPerSecond;
    u32                 FrameCount;

    u32                 LastFPSUpdateFrameCount;
    f32                 LastFPSUpdateTime;
};

struct input
{
    s32 FramebufferWidth, FramebufferHeight;
    vector2 Screen;
    keyboard_input Keyboard;
    mouse_input Mouse;
    time_info Time;

    bool IsPressed(key Key);
    bool IsReleased(key Key);
    bool IsDown(key Key);

    bool IsPressed(mouse_button MouseButton);
    bool IsReleased(mouse_button MouseButton);
    bool IsDown(mouse_button MouseButton);

    f32 DeltaTime();
    f32 ElapsedTime();
};

void InputUpdate(input* Input, input_form* ThisFrame);

#endif //PROCEDURALWORLD_INPUT_H
