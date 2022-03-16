#version 430 core

layout(location = 0) in vec3 AttributePosition;

uniform mat4 Projection, View, Model;

void main() {
    gl_Position = Projection * View * vec4(AttributePosition, 1.0);
}
