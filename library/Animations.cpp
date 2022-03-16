#include "Animations.h"
#include "DebugLog.h"

transition& transition::Create(f32* Value, f32 EndValue, f32 Duration, f32 Delay, ease Ease)
{
    this->Duration = Duration;
    this->CurrentTime = -Delay;

    this->EaseType = Ease;

    this->Type = transition_value_type::Float;
    this->Pointer.Float = Value;
    this->Start.Float = *Value;
    this->End.Float = EndValue;

    return *this;
}

transition& transition::Create(vector2* Value, vector2 EndValue, f32 Duration, f32 Delay, ease Ease)
{
    this->Duration = Duration;
    this->CurrentTime = -Delay;

    this->EaseType = Ease;

    this->Type = transition_value_type::Vector2;
    this->Pointer.Vector2 = Value;
    this->Start.Vector2 = *Value;
    this->End.Vector2 = EndValue;

    return *this;
}

transition& transition::Create(vector3* Value, vector3 EndValue, f32 Duration, f32 Delay, ease Ease)
{
    this->Duration = Duration;
    this->CurrentTime = -Delay;

    this->EaseType = Ease;

    this->Type = transition_value_type::Vector3;
    this->Pointer.Vector3 = Value;
    this->Start.Vector3 = *Value;
    this->End.Vector3 = EndValue;

    return *this;
}

transition& transition_manager::AddTransition(f32* Value, f32 EndValue, f32 Duration, f32 Delay, ease Ease)
{
    transition Transition;
    Transition.Create(Value, EndValue, Duration, Delay, Ease);
    Transitions.push_back(Transition);
    return Transitions.back();
}

transition& transition_manager::AddTransition(vector2* Value, vector2 EndValue, f32 Duration, f32 Delay, ease Ease)
{
    transition Transition;
    Transition.Create(Value, EndValue, Duration, Delay, Ease);
    Transitions.push_back(Transition);
    return Transitions.back();
}

transition& transition_manager::AddTransition(vector3* Value, vector3 EndValue, f32 Duration, f32 Delay, ease Ease)
{
    transition Transition;
    Transition.Create(Value, EndValue, Duration, Delay, Ease);
    Transitions.push_back(Transition);
    return Transitions.back();
}

void transition_manager::Update(f32 DeltaTime)
{
    for (transition& Transition : Transitions)
    {
        Transition.Update(DeltaTime);
    }
}

transition& transition::From(f32 StartValue)
{
    Assert(this->Type == transition_value_type::Float);
    this->Start.Float = StartValue;
    return *this;
}

transition& transition::From(vector2 StartValue)
{
    Assert(this->Type == transition_value_type::Vector2);
    this->Start.Vector2 = StartValue;
    return *this;
}

transition& transition::From(vector3 StartValue)
{
    Assert(this->Type == transition_value_type::Vector3);
    this->Start.Vector3 = StartValue;
    return *this;
}

f32 Ease(ease EaseType, f32 Percent)
{
    switch (EaseType) {
        case ease::QuadIn:        return EaseInQuad(Percent);
        case ease::QuadOut:       return EaseOutQuad(Percent);
        case ease::QuadInOut:     return EaseInOutQuad(Percent);
        case ease::CubicIn:       return EaseInCubic(Percent);
        case ease::CubicOut:      return EaseOutCubic(Percent);
        case ease::CubicInOut:    return EaseInOutCubic(Percent);
        case ease::QuartIn:       return EaseInQuart(Percent);
        case ease::QuartOut:      return EaseOutQuart(Percent);
        case ease::QuartInOut:    return EaseInOutQuart(Percent);
        case ease::QuintIn:       return EaseInQuint(Percent);
        case ease::QuintOut:      return EaseOutQuint(Percent);
        case ease::QuintInOut:    return EaseInOutQuint(Percent);
        case ease::ElasticIn:     return EaseInElastic(Percent);
        case ease::ElasticOut:    return EaseOutElastic(Percent);
        case ease::ElasticInOut:  return EaseInOutElastic(Percent);
        case ease::BackIn:        return EaseInBack(Percent);
        case ease::BackOut:       return EaseOutBack(Percent);
        case ease::BackInOut:     return EaseInOutBack(Percent);
        case ease::BounceIn:      return EaseInBounce(Percent);
        case ease::BounceOut:     return EaseOutBounce(Percent);
        case ease::BounceInOut:   return EaseInOutBounce(Percent);
        case ease::Smooth:        return Smoothstep(Percent);
        case ease::Smoother:      return Smootherstep(Percent);
        case ease::CircIn:        return EaseInCirc(Percent);
        case ease::CircOut:       return EaseOutCirc(Percent);
        case ease::CircInOut:     return EaseInOutCirc(Percent);
        case ease::SineIn:        return EaseInSine(Percent);
        case ease::SineOut:       return EaseOutSine(Percent);
        case ease::SineInOut:     return EaseInOutSine(Percent);
        case ease::Linear:
        default:                return Percent;
    }
}

void transition::Update(f32 DeltaTime)
{
    this->CurrentTime += DeltaTime;

    if(this->CurrentTime < 0.0f) return;

    f32 Percent = Clamp(this->CurrentTime / this->Duration, 0.0f, 1.0f);
    f32 Weight = Ease(this->EaseType, Percent);

    switch (this->Type) {
        case transition_value_type::Float:
            *this->Pointer.Float = Lerp(this->Start.Float, this->End.Float, Weight);
            break;
        case transition_value_type::Vector2:
            *this->Pointer.Vector2 = Lerp(this->Start.Vector2, this->End.Vector2, Weight);
            break;
        case transition_value_type::Vector3:
            *this->Pointer.Vector3 = Lerp(this->Start.Vector3, this->End.Vector3, Weight);
            break;
        default: break;
    }
}


transition& transition::SetEase(ease Ease)
{
    this->EaseType = Ease;
    return *this;
}

transition& transition::SetDelay(f32 Delay)
{
    this->CurrentTime = -Delay;
    return *this;
}

