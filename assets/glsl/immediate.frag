#version 430 core

out vec4 FragmentColor;

uniform sampler2D BoundTexture;

in vec4 Color;
in vec2 UV;

void main()
{
    FragmentColor = Color * texture(BoundTexture, UV);
}
