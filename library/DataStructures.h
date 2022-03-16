#ifndef PROCEDURALWORLD_DATASTRUCTURES_H
#define PROCEDURALWORLD_DATASTRUCTURES_H

#include "Types.h"
#include "platform/Platform.h"
#include "DebugLog.h"

template<typename E, u32 N>
struct array
{
private:
    E Elements[N];
public:
    constexpr u32 Length() const
    {
        return N;
    }

    E& operator[](u32 Index)
    {
        Assert(Index < N);
        return Elements[Index];
    }

    constexpr u32 Bytes() const
    {
        return N * sizeof(E);
    }
};

template<typename E>
struct array_list
{
private:
    E* Elements;
    u32 Count;
    u32 Capacity;
    static const u32 InitialCapacity = 32;
public:
    array_list() : Elements(nullptr), Count(0), Capacity(0) {};

    array_list(u32 InitialCapacity) : Elements(nullptr), Count(0), Capacity(InitialCapacity)
    {
        SetCapacity(InitialCapacity);
    }

    ~array_list()
    {
        PlatformFreeMemory(Elements);
        Capacity = 0;
        Count = 0;
    }
    void Append(const E& Element)
    {
        if(Count == Capacity)
            SetCapacity(Capacity == 0 ? InitialCapacity : Capacity * 2);

        Elements[Count++] = Element;
    }

    void InsertAt(const E& Element, u32 Index)
    {
        Assert(Index <= Count);

        if(Count == Capacity)
            SetCapacity(Capacity == 0 ? InitialCapacity : Capacity * 2);

        for (int i = Index; i < Count - 1; ++i)
            Elements[i + 1] = Elements[i];

        Elements[Index] = Element;
        Count++;
    }

    void RemoveLast()
    {
        Count = Count == 0 ? 0 : Count - 1;
    }

    E& LastElement()
    {
        Assert(Count);
        return Elements[Count - 1];
    }

    u32 Length() const
    {
        return Count;
    }

    u32 Size() const
    {
        return Count * sizeof(E);
    }

    bool IsEmpty() const
    {
        return Length() == 0;
    }

    void Clear()
    {
        PlatformFreeMemory(Elements);
        Elements = nullptr;
        Capacity = 0;
        Count = 0;
    }

    E& operator[](u32 Index)
    {
        Assert((Index < Count));
        return Elements[Index];
    }

    E* GetData()
    {
        return Elements;
    }

private:
    void SetCapacity(u32 NewCapacity)
    {
        Capacity = NewCapacity;
        Elements = (E*)PlatformReallocateMemory(Elements, Capacity * sizeof(E));
    }
};

#endif //PROCEDURALWORLD_DATASTRUCTURES_H
