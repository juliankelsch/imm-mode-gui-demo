#include "Utility.h"
#include <cstdio>
#include "platform/Platform.h"

memory_buffer ReadEntireFile(char* Filepath)
{
    memory_buffer EntireFile = {nullptr, 0};

    FILE* File = fopen(Filepath, "rb");
    if(!File)
    {
        fprintf(stderr, "Error -> Could not open file %s\n", Filepath);
        return EntireFile;
    }

    fseek(File, 0, SEEK_END);
    u64 FileSize = ftell(File);
    fseek(File, 0, SEEK_SET);

    u8* FileContent = (u8*)PlatformAllocateMemory(FileSize + 1);

    if(fread(FileContent, FileSize, 1, File) != 1)
    {
        fprintf(stderr, "Error while loading file %s\n", Filepath);
        return EntireFile;
    }

    FileContent[FileSize] = '\0';

    EntireFile.Data = FileContent;
    EntireFile.Size = FileSize;

    return EntireFile;
}

void WriteToFile(char* Filepath, u8* Data, u32 Size)
{
    FILE* File = fopen(Filepath, "wb");
    if(!File)
    {
        fprintf(stderr, "Error -> Could not open file %s\n", Filepath);
    }
    else if(fwrite(Data, Size, 1, File) != 1)
    {
        fprintf(stderr, "Could not write to file $s\n", Filepath);
    }

    fclose(File);
}

void FreeMemory(memory_buffer* File)
{
    PlatformFreeMemory(File->Data);
    File->Data = nullptr;
    File->Size = 0;
}

u64 Kilobytes(u32 Count)
{
    return (u64)Count * (1 << 10);
}

u64 Megabytes(u32 Count)
{
    return (u64)Count * (1 << 20);
}

u64 Gigabytes(u32 Count)
{
    return (u64)Count * (1 << 30);
}
