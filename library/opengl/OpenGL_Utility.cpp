#include "OpenGL_Utility.h"
#include "glad/glad.h"
#include "Utility.h"
#include <cstdio>

u32 CreateShader(char* Source, u32 Type)
{
    u32 Shader = glCreateShader(Type);
    glShaderSource(Shader, 1, &Source, nullptr);
    glCompileShader(Shader);

    s32 Success;
    glGetShaderiv(Shader, GL_COMPILE_STATUS, &Success);
    if(!Success)
    {
        char InfoLog[512];
        glGetShaderInfoLog(Shader, 512, nullptr, InfoLog);
        fprintf(stderr, "Shader COMPILE error: %s\n", InfoLog);
        return 0;
    }

    return Shader;
}

u32 CreateVertexFragmentShader(char* VertexFilepath, char* FragmentFilepath)
{
    memory_buffer VertexFile = ReadEntireFile(VertexFilepath);
    memory_buffer FragmentFile = ReadEntireFile(FragmentFilepath);

    if(!VertexFile.Data || !FragmentFile.Data)
    {
        return 0;
    }

    u32 VertexShader = CreateShader((char*)VertexFile.Data, GL_VERTEX_SHADER);
    u32 FragmentShader = CreateShader((char*)FragmentFile.Data, GL_FRAGMENT_SHADER);

    u32 Program = glCreateProgram();

    glAttachShader(Program, VertexShader);
    glAttachShader(Program, FragmentShader);

    glLinkProgram(Program);

    s32 Success;
    glGetProgramiv(Program, GL_LINK_STATUS, &Success);
    if(!Success)
    {
        char InfoLog[512];
        glGetProgramInfoLog(Program, 512, nullptr, InfoLog);
        fprintf(stderr, "Program LINK error: %s\n", InfoLog);
        return 0;
    }

    glDeleteShader(VertexShader);
    glDeleteShader(FragmentShader);

    FreeMemory(&VertexFile);
    FreeMemory(&FragmentFile);

    return Program;
}

void SetInt(u32 Program, char* Name, s32 Value)
{
    glUniform1i(glGetUniformLocation(Program, Name), Value);
}

void SetFloat(u32 Program, char* Name, f32 value)
{
    glUniform1f(glGetUniformLocation(Program, Name), value);
}

void SetVec2(u32 Program, char* Name, vector2 Vector)
{
    glUniform2f(glGetUniformLocation(Program, Name), Vector.X, Vector.Y);
}

void SetVec3(u32 Program, char* Name, vector3 Vector)
{
    glUniform3f(glGetUniformLocation(Program, Name), Vector.X, Vector.Y, Vector.Z);
}

void SetVec4(u32 Program, char* Name, vector4 Vector)
{
    glUniform4f(glGetUniformLocation(Program, Name), Vector.X, Vector.Y, Vector.Z, Vector.W);
}

void SetMat4(u32 Program, char* Name, matrix4 Matrix)
{
    glUniformMatrix4fv(glGetUniformLocation(Program, Name), 1, GL_FALSE, Matrix.values);
}
