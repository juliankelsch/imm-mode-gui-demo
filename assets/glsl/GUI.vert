#version 430 core

layout(location = 0) in vec2 AttributePosition;
layout(location = 1) in vec2 AttributeUV;
layout(location = 2) in vec4 AttributeColor;

uniform mat4 ProjectionGUI;

out vec4 Color;
out vec2 UV;

void main()
{
    Color = AttributeColor / 255.0f;
    UV = AttributeUV;
    gl_Position = ProjectionGUI * vec4(AttributePosition, 0.0, 1.0);
}
