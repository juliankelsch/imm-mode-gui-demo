#version 430 core

layout(location = 0) in vec3 AttributePosition;
layout(location = 1) in vec2 AttributeUV;
layout(location = 2) in vec4 AttributeColor;

uniform mat4 ImmediatePVM;

out vec4 Color;
out vec2 UV;

void main()
{
    UV = AttributeUV;
    Color = AttributeColor / 255.0f;
    gl_Position = ImmediatePVM * vec4(AttributePosition, 1.0);
}
