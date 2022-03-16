#ifndef PROCEDURALWORLD_OPENGL_UTILITY_H
#define PROCEDURALWORLD_OPENGL_UTILITY_H

#include "math/Vector.h"
#include "math/Matrix.h"

u32 CreateVertexFragmentShader(char* VertexFilepath, char* FragmentFilepath);
u32 CreateShader(char* Source, u32 Type);

void SetInt(u32 Program, char* Name, s32 Value);
void SetFloat(u32 Program, char* Name, f32 value);
void SetVec2(u32 Program, char* Name, vector2 Vector);
void SetVec3(u32 Program, char* Name, vector3 Vector);
void SetVec4(u32 Program, char* Name, vector4 Vector);
void SetMat4(u32 Program, char* Name, matrix4 Matrix);

#endif //PROCEDURALWORLD_OPENGL_UTILITY_H
