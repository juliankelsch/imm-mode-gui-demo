#include "GUI.h"
#include "platform/Platform.h"
#include "Utility.h"
#include "DebugLog.h"

struct GUI::context
{
    bool IsInitialized;

    render_list_2d RenderList;
    input* Input;
    GUI::font* Font;
    GUI::font* DefaultFont;
    widget_styles Styles;
    GUI::position_mode PositionMode;

    // Note: workspace Vertex and Index Buffer, so that i don't have to add allocate memory constantly
    static const u32 VertexBufferLength = 500;
    vertex_2 VertexBuffer[VertexBufferLength];

    static const u32 IndexBufferLength = 3000;
    u32 IndexBuffer[IndexBufferLength];
};

static GUI::context* Context = nullptr;

void GUI::CreateContext()
{
    if(!Context)
    {
        Context = (GUI::context*)PlatformAllocateMemoryZeroInitialized(sizeof(GUI::context));
        if(!Context)
        {
            LogError("GUI context could not be created!\n");
            return;
        }
        Context->Font = LoadFont("../assets/fonts/Lato-Regular.ttf");
        Context->DefaultFont = Context->Font;
        SetDefaultStyles();
        Context->IsInitialized = true;
    }
}

void GUI::DestroyContext()
{
    Assert(Context);
    GUI::DeleteFont(Context->DefaultFont);
    PlatformFreeMemory(Context->Font);
    PlatformFreeMemory(Context);
    Context = nullptr;
}
void GUI::Line(vector2 Start, vector2 End, color32 Color, f32 LineWidth)
{
    Assert(Context);
    vector2 Direction = End - Start;
    vector2 Offset = Normalize(V(-Direction.Y, Direction.X)) * LineWidth / 2.0f;

    vector2 A = Start - Offset;
    vector2 B = End   - Offset;
    vector2 C = Start + Offset;
    vector2 D = End   + Offset;

    vertex_2 Vertices[] = {
            {A, {0.0f, 0.0f}, Color},
            {B, {0.0f, 0.0f}, Color},
            {C, {0.0f, 0.0f}, Color},
            {D, {0.0f, 0.0f}, Color}
    };
    u32 Indices[] = {
            0, 2, 1,
            3, 1, 2
    };
    Context->RenderList.AddMesh(
            Vertices,
            sizeof(Vertices) / sizeof(*Vertices),
            Indices,
            sizeof(Indices) / sizeof(*Indices),
            0,
            BatchModeColor);

}

static vertex_2 MakeVertex(vector2 Position, vector2 UV, color32 Color)
{
    vertex_2 Vertex = {Position, UV, Color};
    return Vertex;
}

void GUI::LineStrip(vector2* Points, u32 PointCount, color32 Color, f32 LineWidth)
{
    Assert(Context);
    if(PointCount == 1)
    {
        GUI::Rect(Points[0] - V(LineWidth, LineWidth), Points[0] + V(LineWidth, LineWidth), Color);
    }
    else if (PointCount == 2)
    {
        GUI::Line(Points[0], Points[1], Color, LineWidth);
    }
    else
    {
        f32 HalfWidth = LineWidth / 2.0f;
        vertex_2* Vertices      = Context->VertexBuffer;
        vector2 DirectionStart  = Points[1] - Points[0];
        vector2 DirectionEnd    = Points[PointCount - 1] - Points[PointCount - 2];
        vector2 OffsetStart     = Normalize(V(-DirectionStart.Y, DirectionStart.X)) * HalfWidth;
        vector2 OffsetEnd       = Normalize(V(-DirectionEnd.Y, DirectionEnd.X)) * HalfWidth;
        vector2 UV              = {0.0f, 0.0f};

        Vertices[0] = MakeVertex(Points[0] - OffsetStart, UV, Color);
        Vertices[1] = MakeVertex(Points[0] + OffsetStart, UV, Color);

        Vertices[PointCount * 2 - 2] = MakeVertex(Points[PointCount - 1] - OffsetEnd, UV, Color);
        Vertices[PointCount * 2 - 1] = MakeVertex(Points[PointCount - 1] + OffsetEnd, UV, Color);

        for (int I = 1; I < PointCount - 1; ++I)
        {
            vector2 Direction = Normalize(Points[I + 1] - Points[I]) - Normalize(Points[I - 1] - Points[I]);
            vector2 Offset = Normalize(V(-Direction.Y, Direction.X)) * HalfWidth;

            Vertices[I * 2    ] = MakeVertex(Points[I] - Offset, UV, Color);
            Vertices[I * 2 + 1] = MakeVertex(Points[I] + Offset, UV, Color);
        }

        u32* Indices = Context->IndexBuffer;
        u32 BaseIndex = 0;
        for (int I = 0; I < (PointCount - 1) * 6; I += 6)
        {
            Indices[I    ] = BaseIndex + 0;
            Indices[I + 1] = BaseIndex + 1;
            Indices[I + 2] = BaseIndex + 2;

            Indices[I + 3] = BaseIndex + 2;
            Indices[I + 4] = BaseIndex + 1;
            Indices[I + 5] = BaseIndex + 3;
            BaseIndex += 2;
        }

        Context->RenderList.AddMesh(
                Vertices,
                PointCount * 2,
                Indices,
                (PointCount - 1) * 6,
                0,
                BatchModeColor);
    }
}

void GUI::Rect(vector2 Min, vector2 Max, color32 Color)
{
    Assert(Context);
    vertex_2 Vertices[] = {
            {{Min.X, Min.Y}, {0.0f, 0.0f}, Color},
            {{Max.X, Min.Y}, {0.0f, 0.0f}, Color},
            {{Min.X, Max.Y}, {0.0f, 0.0f}, Color},
            {{Max.X, Max.Y}, {0.0f, 0.0f}, Color}
    };
    u32 Indices[] = {
            0, 2, 1,
            3, 1, 2
    };
    Context->RenderList.AddMesh(
            Vertices,
            sizeof(Vertices) / sizeof(*Vertices),
            Indices,
            sizeof(Indices) / sizeof(*Indices),
            0,
            BatchModeColor);
}

void GUI::RoundedRect(vector2 MinPos, vector2 MaxPos, f32 CornerRadius, GUI::corner_mask CornerMask, color32 Color, u32 CornerResolution)
{
    if(CornerMask & ROUNDED_NONE)
    {
        GUI::Rect(MinPos, MaxPos, Color);
    }
    else
    {
        vector2 Size = MaxPos - MinPos;
        f32 Radius = Min(CornerRadius, Min(Size.X / 2.0f, Size.Y / 2.0f));

        vector2 Offset   = {Radius, Radius};
        vector2 TopLeft  = MinPos + Offset;
        vector2 TopRight = {MaxPos.X - Offset.X, MinPos.Y + Offset.Y};
        vector2 BotLeft  = {MinPos.X + Offset.X, MaxPos.Y - Offset.Y};
        vector2 BotRight = MaxPos - Offset;

        GUI::Rect(TopLeft, BotRight, Color);                        // Note: Inner rect
        GUI::Rect(MinPos + V(Offset.X, 0.0f), TopRight, Color);     // Note: Top rect
        GUI::Rect(BotLeft, BotRight + V(0.0f, Offset.Y), Color);    // Note: Bot rect
        GUI::Rect(MinPos + V(0.0f, Offset.Y), BotLeft, Color);      // Note: Left rect
        GUI::Rect(TopRight, BotRight + V(Offset.X, 0.0f), Color);   // Note: Right rect

        if(CornerMask & ROUNDED_TOP_LEFT)
            GUI::CircleSection(TopLeft, Radius, 180.0f, 270.0f, Color, CornerResolution);
        else
            GUI::Rect(MinPos, TopLeft, Color);

        if(CornerMask & ROUNDED_TOP_RIGHT)
            GUI::CircleSection(TopRight, Radius, 270.0f, 360.0f, Color, CornerResolution);
        else
            GUI::Rect(TopRight - V(0.0f, Offset.Y), TopRight + V(Offset.X, 0.0f), Color);

        if(CornerMask & ROUNDED_BOT_LEFT)
            GUI::CircleSection(BotLeft, Radius, 90.0f, 180.0f, Color, CornerResolution);
        else
            GUI::Rect(BotLeft - V(Offset.X, 0.0f), BotLeft + V(0.0f, Offset.Y), Color);

        if(CornerMask & ROUNDED_BOT_RIGHT)
            GUI::CircleSection(BotRight, Radius, 0.0f, 90.0f, Color, CornerResolution);
        else
            GUI::Rect(BotRight, MaxPos, Color);
    }
}


void GUI::Circle(
        vector2         Center,
        f32             Radius,
        color32         Color,
        u32             Resolution)
{
    Assert(Context);
    GUI::CircleSection(Center, Radius, 0.0f, 360.0f, Color, Resolution);
}

void GUI::CircleSection(
        vector2         Center,
        f32             Radius,
        f32             FromAngleDegrees,
        f32             ToAngleDegrees,
        color32         Color,
        u32             Resolution)
{
    Assert(Context);
    Assert((FromAngleDegrees < ToAngleDegrees));
    vertex_2* Vertices = Context->VertexBuffer;
    vector2 UV = {-1.0f, -1.0f};
    Vertices[0] = MakeVertex(Center, UV, Color);

    f32 AngleStart = Deg2Rad(FromAngleDegrees);
    f32 AngleStep  = Deg2Rad(ToAngleDegrees - FromAngleDegrees) / Resolution;
    for (u32 I = 0; I < Resolution + 1; ++I)
    {
        f32 Angle = AngleStart + I * AngleStep;
        vector2 Position = Center + Radius * V(Cos(Angle), Sin(Angle));
        Vertices[I + 1]  = MakeVertex(Position, UV, Color);
    }

    u32* Indices = Context->IndexBuffer;
    u32 Offset = 0;
    for (int I = 0; I < Resolution * 3; I += 3)
    {
        Indices[I    ] = 0;
        Indices[I + 1] = Offset + 2;
        Indices[I + 2] = Offset + 1;
        Offset++;
    }
    Context->RenderList.AddMesh(
            Vertices,
            Resolution + 2,
            Indices,
            Resolution * 3,
            0,
            BatchModeColor);
}

void GUI::StartFrame(input* Input)
{
    Assert(Context);
    Context->Input = Input;
    Context->RenderList.Clear();
}

void GUI::EndFrame()
{
    Assert(Context);

}

render_list_2d* GUI::GetRenderList()
{
    Assert(Context);
    return &Context->RenderList;
}

GUI::image GUI::LoadImage(void* Pixels, u32 Channels, u32 Width, u32 Height, GUI::image_interpolation Interpolation)
{
    Assert(Context);
    GUI::image Image = {};
    Image.DeviceHandle = GUI::CreateDeviceTexture(Pixels, Channels, Width, Height);
    if(Image.DeviceHandle)
    {
        Image.Width = Width;
        Image.Channels = Channels;
        Image.Height = Height;
        Image.Interpolation = Interpolation;
    }
    else
    {
        LogWarn("GUI::texture could not be created!\n");
    }
    return Image;
}

void GUI::Image(GUI::image* image, vector2 Position, f32 Scale)
{
    Assert(Context);
    f32 Width = image->Width * Scale;
    f32 Height = image->Height * Scale;
    vector2 Min = Position;
    vector2 Max {Min.X + Width, Min.Y + Height};
    color32 Color = {255, 255, 255, 255};
    vertex_2 Vertices[] = {
            {{Min.X, Min.Y}, {0.0f, 0.0f}, Color},
            {{Max.X, Min.Y}, {1.0f, 0.0f}, Color},
            {{Min.X, Max.Y}, {0.0f, 1.0f}, Color},
            {{Max.X, Max.Y}, {1.0f, 1.0f}, Color}
    };
    u32 Indices[] = {
            0, 2, 1,
            3, 1, 2
    };
    bool Bitmap = image->Channels == 1;
    Context->RenderList.AddMesh(
            Vertices,
            sizeof(Vertices) / sizeof(*Vertices),
            Indices,
            sizeof(Indices) / sizeof(*Indices),
            image->DeviceHandle,
            Bitmap);
}

GUI::font* GUI::LoadFont(void* TrueTypeBuffer, u32 Size, f32 FontSize)
{
    Assert(Context);
    u8* Bitmap = (u8*)PlatformAllocateMemoryZeroInitialized(512 * 512);
    GUI::font* NewFont = (GUI::font*)PlatformAllocateMemory(sizeof(GUI::font));
    stbtt_BakeFontBitmap((u8*)TrueTypeBuffer, 0, FontSize, Bitmap, 512, 512, 32, 96, NewFont->CharacterInfos);
    NewFont->TextureDeviceHandle = GUI::CreateDeviceTexture(Bitmap, 1, 512, 512);
    PlatformFreeMemory(Bitmap);
    NewFont->Size = FontSize;
    f32 MinY = 9999999999.0f, MaxY = -99999999.9f;
    vector2 Cursor = {0.0f, FontSize};
    for (int Character = 32; Character < 128; ++Character)
    {
        stbtt_aligned_quad Quad;
        stbtt_GetBakedQuad(NewFont->CharacterInfos, 512, 512, Character - 32, &Cursor.X, &Cursor.Y, &Quad, 1);
        if(Quad.y0 < MinY)
        {
            MinY = Quad.y0;
        }

        if(Quad.y1 > MaxY)
        {
            MaxY = Quad.y1;
        }
    }
    NewFont->CursorOffset = MinY;
    NewFont->MaxOverhang = MaxY - FontSize;
    NewFont->Height = FontSize - MinY + NewFont->MaxOverhang;
    return NewFont;
}

GUI::font* GUI::LoadFont(char* TrueTypeFilepath, f32 FontSize)
{
    Assert(Context);
    memory_buffer TrueTypeFile = ReadEntireFile(TrueTypeFilepath);
    return LoadFont(TrueTypeFile.Data, TrueTypeFile.Size, FontSize);
}
f32 GUI::GetCurrentFontSize()
{
    Assert(Context);
    return Context->Font->Size;
}

void GUI::DeleteFont(GUI::font* Font)
{
    Assert(Context);
}

void GUI::SetFont(GUI::font* Font)
{
    Assert(Context);
    if(Font != nullptr)
    {
        Context->Font = Font;
    }
}

void GUI::SetDefaultFont()
{
    Assert(Context);
    Context->Font = Context->DefaultFont;
}
static vector2 CalculateTextBoundingBoxSize(char* Text)
{
    vector2 Cursor = {0.0f, Context->Font->Size - Context->Font->CursorOffset};
    f32 LineStart = Cursor.X;
    while(*Text)
    {
        if(*Text >= 32 && *Text < 128)
        {
            stbtt_aligned_quad Quad;
            stbtt_GetBakedQuad(Context->Font->CharacterInfos, 512, 512, *Text - 32, &Cursor.X, &Cursor.Y, &Quad, 1);
        }
        else if(*Text == '\n')
        {
            Cursor.Y += Context->Font->Size;
            Cursor.X = LineStart;
        }
        Text++;
    }
    Cursor.Y += Context->Font->MaxOverhang;
    return Cursor;
}


void GUI::Text(vector2 TopLeft, char* String, color32 Color)
{
    Assert(Context);
    f32 LineStart = TopLeft.X;
    TopLeft.Y += Context->Font->Size - Context->Font->CursorOffset;
    while(*String)
    {
        if(*String >= 32 && *String < 128)
        {
            stbtt_aligned_quad Quad;
            stbtt_GetBakedQuad(Context->Font->CharacterInfos, 512, 512, *String - 32, &TopLeft.X, &TopLeft.Y, &Quad, 1);
            vertex_2 Vertices[] = {
                    {{Quad.x0, Quad.y0}, {Quad.s0, Quad.t0}, Color},
                    {{Quad.x1, Quad.y0}, {Quad.s1, Quad.t0}, Color},
                    {{Quad.x0, Quad.y1}, {Quad.s0, Quad.t1}, Color},
                    {{Quad.x1, Quad.y1}, {Quad.s1, Quad.t1}, Color}
            };
            u32 Indices[] = {
                    0, 2, 1,
                    3, 1, 2
            };
            Context->RenderList.AddMesh(
                    Vertices,
                    sizeof(Vertices) / sizeof(*Vertices),
                    Indices,
                    sizeof(Indices) / sizeof(*Indices),
                    Context->Font->TextureDeviceHandle,
                    true);
        }
        else if(*String == '\n')
        {
           TopLeft.Y += Context->Font->Size;
           TopLeft.X = LineStart;
        }
        String++;
    }

}

void GUI::TextCentered(vector2 Center, char* String, color32 Color)
{
    Assert(Context);
    vector2 TextBox = CalculateTextBoundingBoxSize(String);
    vector2 TopLeft = Center - (TextBox / 2.0f);
    f32 LineStart = TopLeft.X;
    TopLeft.Y += Context->Font->Size - Context->Font->CursorOffset;
    while(*String)
    {
        if(*String >= 32 && *String < 128)
        {
            stbtt_aligned_quad Quad;
            stbtt_GetBakedQuad(Context->Font->CharacterInfos, 512, 512, *String - 32, &TopLeft.X, &TopLeft.Y, &Quad, 1);
            vertex_2 Vertices[] = {
                    {{Quad.x0, Quad.y0}, {Quad.s0, Quad.t0}, Color},
                    {{Quad.x1, Quad.y0}, {Quad.s1, Quad.t0}, Color},
                    {{Quad.x0, Quad.y1}, {Quad.s0, Quad.t1}, Color},
                    {{Quad.x1, Quad.y1}, {Quad.s1, Quad.t1}, Color}
            };
            u32 Indices[] = {
                    0, 2, 1,
                    3, 1, 2
            };
            Context->RenderList.AddMesh(
                    Vertices,
                    sizeof(Vertices) / sizeof(*Vertices),
                    Indices,
                    sizeof(Indices) / sizeof(*Indices),
                    Context->Font->TextureDeviceHandle,
                    true);
        }
        else if(*String == '\n')
        {
            TopLeft.Y += Context->Font->Size;
            TopLeft.X = LineStart;
        }
        String++;
    }
}

static void ConvertToMinMax(vector2 Position, vector2 Size, vector2* Min, vector2* Max)
{
    if(Context->PositionMode == GUI::PositionCenter)
    {
        *Min = V(Position.X - Size.X / 2.0f, Position.Y - Size.Y / 2.0f);
        *Max = V(Position.X + Size.X / 2.0f, Position.Y + Size.Y / 2.0f);
    }
    else
    {
        *Min = Position;
        *Max = Position + Size;
    }
}

static bool IsInsideRect(vector2 Point, vector2 Min, vector2 Max)
{
    return Point.X >= Min.X && Point.X <= Max.X && Point.Y >= Min.Y && Point.Y <= Max.Y;
}

void GUI::SetStyles(GUI::widget_styles* Styles)
{
    Assert(Context);
    memcpy(&Context->Styles, Styles, sizeof(GUI::widget_styles));
}

void GUI::SetDefaultStyles()
{
    Assert(Context);
    GUI::widget_styles* Styles = &Context->Styles;
    color32 WidgetColor = Color(0.5f, 0.2f, 0.1f, 1.0f);
    color32 ActiveWidgetColor = Color(0.75f, 0.3f, 0.15f, 1.0f);
    color32 WidgetTextColor = Color(0.7f, 0.7f, 0.7f, 1.0f);
    color32 ActiveWidgetTextColor = Color(1.0f, 1.0f, 1.0f, 1.0f);

    Styles->ButtonColor = WidgetColor;
    Styles->ButtonHighlightColor = ActiveWidgetColor;
    Styles->ButtonTextColor = WidgetTextColor;
    Styles->ButtonTextHighlightColor = ActiveWidgetTextColor;
}

static GUI::widget_styles* GetStyles()
{
    Assert(Context);
    return &Context->Styles;
}

void GUI::SetPositionMode(position_mode Mode)
{
    Assert(Context);
    Context->PositionMode = Mode;
}

bool GUI::Button(char* Description, vector2 Position, vector2 Size)
{
    Assert(Context);
    vector2 Min, Max;
    ConvertToMinMax(Position, Size, &Min, &Max);
    vector2 Center = (Min + Max) / 2.0f;
    mouse_input* Mouse = &Context->Input->Mouse;

    GUI::widget_styles* Styles = GetStyles();
    bool IsClicked = false;
    vector2 TextBox = CalculateTextBoundingBoxSize(Description);
    vector2 TextPosition = Center - (TextBox / 2.0f);

    if(IsInsideRect(Mouse->Position, Min, Max))
    {
        GUI::Rect(Min, Max, Styles->ButtonHighlightColor);
        GUI::Text(TextPosition, Description, Styles->ButtonTextHighlightColor);
        if(Mouse->ButtonInfos[MouseButtonLeft].State == Pressed)
        {
            IsClicked = true;
        }
    }
    else
    {
        GUI::Rect(Min, Max, Styles->ButtonColor);
        GUI::Text(TextPosition, Description, Styles->ButtonTextColor);
    }
    return IsClicked;
}

static bool IsInsideCircle(vector2 Point, vector2 CircleCenter, f32 CircleRadius)
{
    return Dot(Point - CircleCenter, Point - CircleCenter) < (CircleRadius * CircleRadius);

}

bool GUI::Checkbox(char* Description, bool* Value, vector2 Position)
{
    f32 CheckboxOffset = Context->Font->Height / 2.0f;
    f32 Radius = 1.2f * CheckboxOffset;
    vector2 CheckboxCenter = Position + V(CheckboxOffset, CheckboxOffset);
    mouse_input* Mouse = &Context->Input->Mouse;
    if(IsInsideCircle(Mouse->Position, CheckboxCenter, Radius))
    {
        GUI::Circle(CheckboxCenter, Radius, Color(0.25f, 0.25f, 0.25f, 1.0f));
        if(Mouse->ButtonInfos[MouseButtonLeft].State == Released)
        {
            *Value = !(*Value);
        }
        if(*Value)
        {
            GUI::Circle(CheckboxCenter, 0.7f * Radius, Color(0.75f, 0.3f, 0.15f, 1.0f));
        }
    }
    else
    {
        GUI::Circle(CheckboxCenter, Radius, Color(0.15f, 0.15f, 0.15f, 1.0f));
        if(*Value)
        {
            GUI::Circle(CheckboxCenter, 0.7f * Radius, Color(0.5f, 0.2f, 0.1f, 1.0f));
        }
    }


    f32 TextOffset = 10.0f;
    GUI::Text(Position + V(CheckboxOffset * 2.0f + TextOffset, 0.0f), Description, Color(1.0f, 1.0f, 1.0f, 1.0f));
    return *Value;
}

bool GUI::SliderInt(char* Description, u32* Value, u32 Low, u32 High, vector2 Position, f32 Width)
{
    mouse_input* Mouse = &Context->Input->Mouse;

    f32 BarWidth = 6.0f;
    vector2 Center = {Map(*Value, Low, High, Position.X, Position.X + Width), Position.Y + Context->Font->Height / 2.0f};
    bool IsHandleMoved = false;
    if(IsInsideRect(Mouse->Position, Position, Position + V(Width, Context->Font->Height)))
    {
        GUI::Rect(V(Position.X, Center.Y - BarWidth / 2.0f), V(Position.X + Width, Center.Y + BarWidth / 2.0f), Color(0.25f, 0.25f, 0.25f, 1.0f));
        button_state State = Mouse->ButtonInfos[MouseButtonLeft].State;
        if(State == Pressed || State == Held)
        {
            Center.X = Mouse->Position.X;
            IsHandleMoved = true;
        }
    }
    else
    {
        GUI::Rect(V(Position.X, Center.Y - BarWidth / 2.0f), V(Position.X + Width, Center.Y + BarWidth / 2.0f), Color(0.15f, 0.15f, 0.15f, 1.0f));
    }

    f32 SliderValue = Map(Center.X, Position.X, Position.X + Width, Low, High);
    *Value = (u32)(SliderValue + 0.5f);
    *Value = *Value < Low ? Low :  (*Value > High ? High : *Value);
    Center.X = Map(*Value, Low, High, Position.X, Position.X + Width);

    GUI::Circle(Center, Context->Font->Height / 2.0f, IsHandleMoved ? Color(0.75f, 0.3f, 0.15f, 1.0f) : Color(0.5f, 0.2f, 0.1f, 1.0f));

    f32 TextOffset = 15.0f;
    StringF(NumberText, "%d", *Value);
    GUI::Text(Position + V(Width + TextOffset, 0.0f), NumberText);
    GUI::Text(Position + V(Width + TextOffset * 6, 0.0f), Description);
    return IsHandleMoved;
}


bool GUI::SliderInt(char* Description, u8* Value, u8 Low, u8 High, vector2 Position, f32 Width)
{
    mouse_input* Mouse = &Context->Input->Mouse;

    f32 BarWidth = 6.0f;
    vector2 Center = {Map(*Value, Low, High, Position.X, Position.X + Width), Position.Y + Context->Font->Height / 2.0f};
    bool IsHandleMoved = false;
    if(IsInsideRect(Mouse->Position, Position, Position + V(Width, Context->Font->Height)))
    {
        GUI::Rect(V(Position.X, Center.Y - BarWidth / 2.0f),
                  V(Position.X + Width, Center.Y + BarWidth / 2.0f),
                  Color(0.25f, 0.25f, 0.25f, 1.0f));
        button_state State = Mouse->ButtonInfos[MouseButtonLeft].State;
        if(State == Pressed || State == Held)
        {
            Center.X = Mouse->Position.X;
            IsHandleMoved = true;
        }
    }
    else
    {
        GUI::Rect(V(Position.X, Center.Y - BarWidth / 2.0f),
                  V(Position.X + Width, Center.Y + BarWidth / 2.0f),
                  Color(0.15f, 0.15f, 0.15f, 1.0f));
    }

    f32 SliderValue = Map(Center.X, Position.X, Position.X + Width, Low, High);
    *Value = (u32)(SliderValue + 0.5f);
    *Value = *Value < Low ? Low :  (*Value > High ? High : *Value);
    Center.X = Map(*Value, Low, High, Position.X, Position.X + Width);

    GUI::Circle(Center, Context->Font->Height / 2.0f,
            IsHandleMoved
            ? Color(0.75f, 0.3f, 0.15f, 1.0f) :
            Color(0.5f, 0.2f, 0.1f, 1.0f));

    f32 TextOffset = 15.0f;
    StringF(NumberText, "%d", *Value);
    GUI::Text(Position + V(Width + TextOffset, 0.0f), NumberText);
    GUI::Text(Position + V(Width + TextOffset * 6, 0.0f), Description);
    return IsHandleMoved;
}

bool GUI::SliderFloat(char* Description, f32* Value, f32 Low, f32 High, vector2 Position, f32 Width)
{
    mouse_input* Mouse = &Context->Input->Mouse;

    f32 BarWidth = 6.0f;
    vector2 Center = {Map(*Value, Low, High, Position.X, Position.X + Width), Position.Y + Context->Font->Height / 2.0f};
    bool IsHandleMoved = false;
    if(IsInsideRect(Mouse->Position, Position, Position + V(Width, Context->Font->Height)))
    {
        GUI::Rect(V(Position.X, Center.Y - BarWidth / 2.0f),
                  V(Position.X + Width, Center.Y + BarWidth / 2.0f),
                  Color(0.25f, 0.25f, 0.25f, 1.0f));
        button_state State = Mouse->ButtonInfos[MouseButtonLeft].State;
        if(State == Pressed || State == Held)
        {
            Center.X = Mouse->Position.X;
            IsHandleMoved = true;
        }
    }
    else
    {
        GUI::Rect(V(Position.X, Center.Y - BarWidth / 2.0f),
                  V(Position.X + Width, Center.Y + BarWidth / 2.0f),
                  Color(0.15f, 0.15f, 0.15f, 1.0f));
    }

    f32 SliderValue = Map(Center.X, Position.X, Position.X + Width, Low, High);
    *Value = Clamp(SliderValue, Low, High);
    Center.X = Map(*Value, Low, High, Position.X, Position.X + Width);

    GUI::Circle(Center, Context->Font->Height / 2.0f, IsHandleMoved ? Color(0.75f, 0.3f, 0.15f, 1.0f) : Color(0.5f, 0.2f, 0.1f, 1.0f));

    f32 TextOffset = 15.0f;
    StringF(NumberText, "%.3g", *Value);
    GUI::Text(Position + V(Width + TextOffset, 0.0f), NumberText);
    GUI::Text(Position + V(Width + TextOffset * 6, 0.0f), Description);
    return IsHandleMoved;
}

bool GUI::SliderColorRGBA(char* Description, color32* Value, vector2 Position, f32 Width)
{
    GUI::Text(Position, Description);
    return
          GUI::SliderInt("R", &Value->R, 0, 255, Position + V(0.0f,     Context->Font->Size))
        | GUI::SliderInt("G", &Value->G, 0, 255, Position + V(0.0f, 2 * Context->Font->Size))
        | GUI::SliderInt("B", &Value->B, 0, 255, Position + V(0.0f, 3 * Context->Font->Size))
        | GUI::SliderInt("A", &Value->A, 0, 255, Position + V(0.0f, 4 * Context->Font->Size));
}







