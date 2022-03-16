#ifndef PROCEDURALWORLD_PLATFORM_H
#define PROCEDURALWORLD_PLATFORM_H

#include "Types.h"

struct application_settings
{
    u32 WindowWidth;
    u32 WindowHeight;
    char* WindowTitle;

    u32 OpenGLVersionMajor;
    u32 OpenGLVersionMinor;

    bool UseMultisampling;
    u32 Samples;
    bool UseVSync;
};

// Note: Platform specific ways of allocating memory.
void* PlatformAllocateMemory(u32 NumberOfBytes);
void* PlatformAllocateMemoryZeroInitialized(u32 NumberOfBytes);
void* PlatformReallocateMemory(void* Memory, u32 NumberOfBytes);
void PlatformFreeMemory(void* Memory);

void PlatformToggleFullscreen();
void PlatformExit();

// Note: This function is supposed to apply the wanted settings and call GameUpdateAndRender().
s32 PlatformRunApplication(application_settings* Settings); // Note: Returns a return code for the main function

#endif //PROCEDURALWORLD_PLATFORM_H
