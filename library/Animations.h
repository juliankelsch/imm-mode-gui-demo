#ifndef PROCEDURALWORLD_ANIMATIONS_H
#define PROCEDURALWORLD_ANIMATIONS_H
#include "Types.h"
#include "CGMath.h"

enum class transition_value_type : u32
{
    Float, Vector2, Vector3
};

enum class ease : u32
{
    Linear,
    Smooth, Smoother,
    BounceIn, BounceOut, BounceInOut,
    BackIn, BackOut, BackInOut,
    QuadIn, QuadOut, QuadInOut,
    CubicIn, CubicOut, CubicInOut,
    QuartIn, QuartOut, QuartInOut,
    QuintIn, QuintOut, QuintInOut,
    ElasticIn, ElasticOut, ElasticInOut,
    SineIn, SineOut, SineInOut,
    CircIn, CircOut, CircInOut,
    Default = Linear
};

f32 Ease(ease EaseType, f32 Percent);

struct transition {
public:
    transition& Create(f32* Value, f32 EndValue, f32 Duration = 1.0f, f32 Delay = 0.0f, ease Ease = ease::Default);
    transition& Create(vector2* Value, vector2 EndValue, f32 Duration = 1.0f, f32 Delay = 0.0f, ease Ease = ease::Default);
    transition& Create(vector3* Value, vector3 EndValue, f32 Duration = 1.0f, f32 Delay = 0.0f, ease Ease = ease::Default);

    transition& From(f32 StartValue);
    transition& From(vector2 StartValue);
    transition& From(vector3 StartValue);

    transition& SetEase(ease Ease);
    transition& SetDelay(f32 Delay);

    void Update(f32 DeltaTime);
private:
    f32 Duration;
    f32 CurrentTime;
    ease EaseType;
    transition_value_type Type;
    union {
        f32* Float;
        vector2* Vector2;
        vector3* Vector3;
    } Pointer;
    union {
        f32 Float;
        vector2 Vector2;
        vector3 Vector3;
    } Start;
    union {
        f32 Float;
        vector2 Vector2;
        vector3 Vector3;
    } End;
};

#include <vector>
struct transition_manager
{
public:
    transition& AddTransition(f32* Value, f32 EndValue, f32 Duration = 1.0f, f32 Delay = 0.0f, ease Ease = ease::Default);
    transition& AddTransition(vector2* Value, vector2 EndValue, f32 Duration = 1.0f, f32 Delay = 0.0f, ease Ease = ease::Default);
    transition& AddTransition(vector3* Value, vector3 EndValue, f32 Duration = 1.0f, f32 Delay = 0.0f, ease Ease = ease::Default);

    void Update(f32 DeltaTime);
private:
    std::vector<transition> Transitions;
};

#endif //PROCEDURALWORLD_ANIMATIONS_H
