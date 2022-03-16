#ifndef PROCEDURALWORLD_GUI_H
#define PROCEDURALWORLD_GUI_H

#include "math/CGMath.h"
#include "Input.h"
#include "RenderList2D.h"
#include "../stb/stb_truetype.h"

namespace GUI
{
    //>>>>>>>>>>>>>>>>>>>>>> GENERAL SETUP <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    struct context;

    void CreateContext();
    void DestroyContext();

    void StartFrame(input* Input);
    void EndFrame();
    render_list_2d* GetRenderList();

    //>>>>>>>>>>>>>>>>>>>>>> RENDERER INTERFACE <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    // Note: Implemented by Renderer in separate .cpp file
    void SetupRenderer();
    void ShutdownRenderer();
    u32 CreateDeviceFontTexture(byte* Image8, u32 Width, u32 Height);
    u32 CreateDeviceTexture(void* Data, u32 Channels, u32 Width, u32 Height);
    void Render(u32 FramebufferWidth, u32 FramebufferHeight, render_list_2d* RenderList);

    // >>>>>>>>>>>>>>>>>> PRIMITIVES <<<<<<<<<<<<<<<<<<<<<<<<<
    void Rect(vector2 Min, vector2 Max, color32 Color = {255, 255, 255, 255});
    void Line(vector2 Start, vector2 End, color32 Color = {255, 255, 255, 255}, f32 LineWidth = 1.0f);
    void LineStrip(vector2* Points, u32 PointCount, color32 Color = {255, 255, 255, 255}, f32 LineWidth = 1.0f);

    enum corner_mask
    {
        ROUNDED_NONE = 0x0,
        ROUNDED_TOP_LEFT = 0x1,
        ROUNDED_TOP_RIGHT = 0x4,
        ROUNDED_BOT_LEFT = 0x8,
        ROUNDED_BOT_RIGHT = 0x10,
        ROUNDED_TOP = ROUNDED_TOP_LEFT | ROUNDED_TOP_RIGHT,
        ROUNDED_BOT = ROUNDED_BOT_LEFT | ROUNDED_BOT_RIGHT,
        ROUNDED_LEFT = ROUNDED_BOT_LEFT | ROUNDED_TOP_LEFT,
        ROUNDED_RIGHT = ROUNDED_BOT_RIGHT | ROUNDED_TOP_RIGHT,
        ROUNDED_ALL = ROUNDED_TOP | ROUNDED_BOT,
    };
    void RoundedRect(
            vector2         MinPos,
            vector2         MaxPos,
            f32             CornerRadius = 4.0f,
            corner_mask     CornerMask = ROUNDED_ALL,
            color32         Color = {255, 255, 255, 255},
            u32             CornerResolution = 10);

    void Circle(
            vector2         Center,
            f32             Radius,
            color32         Color = {255, 255, 255, 255},
            u32             Resolution = 25);

    void CircleSection(
            vector2         Center,
            f32             Radius,
            f32             FromAngleDegrees, // Note: Clockwise
            f32             ToAngleDegrees,
            color32         Color = {255, 255, 255, 255},
            u32             Resolution = 25);

    //>>>>>>>>>>>>>>>>>>>> FONT <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    struct font
    {
        stbtt_bakedchar CharacterInfos[96];
        u32 TextureDeviceHandle;
        f32 Size;
        //Note: Max Overhang is the max offset below the baseline of a character
        //Note: Both values start from the baseline in their directions
        f32 CursorOffset;
        f32 MaxOverhang;
        f32 Height; // Note: CompleteSize from Min to Max pixel
    };
    font* LoadFont(void* TrueTypeBuffer, u32 Size, f32 FontSize = 15.0f);
    font* LoadFont(char* TrueTypeFilepath, f32 FontSize = 15.0f);
    void DeleteFont(font* Font);
    void SetFont(font* Font);
    void SetDefaultFont();
    f32 GetCurrentFontSize();
    void Text(vector2 TopLeft, char* String, color32 Color = {255, 255, 255, 255});
    void TextCentered(vector2 Center, char* String, color32 Color = {255, 255, 255, 255});

    //>>>>>>>>>>>>>>>>>>>> IMAGES <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    enum image_interpolation {NearestNeighbour, Linear, Bilinear};
    struct image
    {
        u32 DeviceHandle;
        u32 Width, Height, Channels;
        image_interpolation Interpolation;
    };
    void SetInterpolation(image* Image, image_interpolation Interpolation);
    image LoadImage(void* Pixels, u32 Channels, u32 Width, u32 Height, image_interpolation Interpolation = NearestNeighbour);
    void Image(image* image, vector2 Position, f32 Scale = 1.0f);

    //>>>>>>>>>>>>>>>>>>>>>> WIDGETS <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    struct widget_styles
    {
        color32 ButtonColor;
        color32 ButtonTextColor;
        color32 ButtonHighlightColor;
        color32 ButtonTextHighlightColor;
    };
    void SetStyles(widget_styles* Styles);
    void SetDefaultStyles();
    enum position_mode{PositionCenter, PositionTopLeft};
    void SetPositionMode(position_mode Mode);

    bool Button(char* Description, vector2 Position, vector2 Size);
    //Note: Position is top left/ height is current font size
    bool Checkbox(char* Description, bool* Value, vector2 Position);
    // Note: Returns true if the value changes
    bool SliderInt(char* Description, u32* Value, u32 Low, u32 High, vector2 Position, f32 Width = 180.0f);
    bool SliderInt(char* Description, u8* Value, u8 Low, u8 High, vector2 Position, f32 Width = 180.0f);

    bool SliderFloat(char* Description, f32* Value, f32 Low, f32 High, vector2 Position, f32 Width = 180.0f);
    bool SliderColorRGBA(char* Description, color32* Value, vector2 Position, f32 Width = 180.0f);
}

#endif //PROCEDURALWORLD_GUI_H
