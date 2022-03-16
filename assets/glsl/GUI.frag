#version 430 core

out vec4 FragmentColor;

#define MODE_COLOR 0
#define MODE_IMAGE 1
#define MODE_BITMAP 2

uniform int Mode;
uniform sampler2D Image;
uniform bool UseBitmap;
in vec4 Color;
in vec2 UV;

void main()
{
    if(Mode == MODE_COLOR)
        FragmentColor = Color;
    else if (Mode == MODE_BITMAP)
        FragmentColor = vec4(Color.rgb, Color.a * texture(Image, UV).r);
    else if(Mode == MODE_IMAGE)
        FragmentColor = texture(Image, UV);
}
