#ifndef PROCEDURALWORLD_UTILITY_H
#define PROCEDURALWORLD_UTILITY_H

#include "Types.h"

#define ARRAY_COUNT(Array) (sizeof((Array)) / sizeof(*(Array)))
#define StringF(Name, Format, ...) char Name[256] = {}; sprintf(Name, Format, __VA_ARGS__)

struct memory_buffer {
    u8* Data;
    u32 Size;
};

memory_buffer ReadEntireFile(char* Filepath);
void FreeMemory(memory_buffer* File);
void WriteToFile(char* Filepath, u8* Data, u32 Size);

u64 Kilobytes(u32 Count);
u64 Megabytes(u32 Count);
u64 Gigabytes(u32 Count);

#endif //PROCEDURALWORLD_UTILITY_H
