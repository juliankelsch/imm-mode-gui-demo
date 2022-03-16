#include "Input.h"

void InputUpdate(input* Input, input_form* ThisFrame) {
    Input->FramebufferWidth = ThisFrame->FramebufferWidth;
    Input->FramebufferHeight = ThisFrame->FramebufferHeight;
    Input->Screen = V(Input->FramebufferWidth, Input->FramebufferHeight);

    vector2 NewMousePosition = V((f32) ThisFrame->MouseX, (f32) ThisFrame->MouseY);
    Input->Mouse.Motion = NewMousePosition - Input->Mouse.Position;
    Input->Mouse.Position = NewMousePosition;

    for (int Key = 0; Key < KeyCount; ++Key)
    {
        button_info* KeyInfo = &Input->Keyboard.KeyInfos[Key];
        button_state LastState = KeyInfo->State;
        bool IsPressed = ThisFrame->KeysPressed[Key];

        if(LastState == None && IsPressed)
        {
            KeyInfo->State = Pressed;
        }
        else if(LastState == Pressed && IsPressed)
        {
            KeyInfo->State = Held;
        }
        else if(LastState == Pressed && !IsPressed)
        {
            KeyInfo->State = Released;
        }
        else if(LastState == Held && IsPressed)
        {
            KeyInfo->State = Held;
        }
        else if(LastState == Held && !IsPressed)
        {
            KeyInfo->State = Released;
        }
        else if(LastState == Released && IsPressed)
        {
            KeyInfo->State = Pressed;
        }
        else
        {
            KeyInfo->State = None;
        }
    }

    for (int MouseButton = 0; MouseButton < MouseButtonCount; ++MouseButton)
    {
        button_info* ButtonInfo = &Input->Mouse.ButtonInfos[MouseButton];
        button_state LastState = ButtonInfo->State;
        bool IsPressed = ThisFrame->MouseButtonsPressed[MouseButton];

        if(LastState == None && IsPressed)
        {
            ButtonInfo->State = Pressed;
        }
        else if(LastState == Pressed && IsPressed)
        {
            ButtonInfo->State = Held;
        }
        else if(LastState == Pressed && !IsPressed)
        {
            ButtonInfo->State = Released;
        }
        else if(LastState == Held && IsPressed)
        {
            ButtonInfo->State = Held;
        }
        else if(LastState == Held && !IsPressed)
        {
            ButtonInfo->State = Released;
        }
        else if(LastState == Released && IsPressed)
        {
            ButtonInfo->State = Pressed;
        }
        else
        {
            ButtonInfo->State = None;
        }
    }

    time_info* TimeInfo = &Input->Time;
    TimeInfo->DeltaTime = (f32)ThisFrame->Time - TimeInfo->ElapsedTime;
    TimeInfo->ElapsedTime = (f32)ThisFrame->Time;
    TimeInfo->FrameCount++;

    if(TimeInfo->LastFPSUpdateTime + 0.5f < TimeInfo->ElapsedTime)
    {
        TimeInfo->FramesPerSecond = (TimeInfo->FrameCount - TimeInfo->LastFPSUpdateFrameCount) / (TimeInfo->ElapsedTime - TimeInfo->LastFPSUpdateTime);
        TimeInfo->LastFPSUpdateTime = TimeInfo->ElapsedTime;
        TimeInfo->LastFPSUpdateFrameCount = TimeInfo->FrameCount;
    }
}

bool input::IsPressed(key Key)
{
    return Keyboard.KeyInfos[Key].State == button_state::Pressed;
}
bool input::IsReleased(key Key)
{
    return Keyboard.KeyInfos[Key].State == button_state::Released;
}

bool input::IsDown(key Key)
{
    button_state KeyState = Keyboard.KeyInfos[Key].State;
    return KeyState == button_state::Pressed || KeyState == button_state::Held;
}

bool input::IsPressed(mouse_button MouseButton)
{
    return Mouse.ButtonInfos[MouseButton].State == button_state::Pressed;
}

bool input::IsReleased(mouse_button MouseButton)
{
    return Mouse.ButtonInfos[MouseButton].State == button_state::Released;
}

bool input::IsDown(mouse_button MouseButton)
{
    return Mouse.ButtonInfos[MouseButton].State == button_state::Pressed
        || Mouse.ButtonInfos[MouseButton].State == button_state::Held;
}

f32 input::DeltaTime()
{
    return Time.DeltaTime;
}

f32 input::ElapsedTime()
{
    return Time.ElapsedTime;
}
