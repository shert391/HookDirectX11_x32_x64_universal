#include <windows.h>
#include "Hooks.h"
#include <iostream>
typedef __int64 QWORD;

#ifdef _WIN64
void* Hook::hook1(void* src, OUT void* var)
{
    locationOne = (void*)((QWORD)src + 0x21);

    DWORD oldProtect{};

    byte stub[] = { 0x48, 0x89, 0x08, 0x58, 0x48, 0x33, 0xC4, 0x48, 0x89, 0x45, 0x60, 0x45, 0x33, 0xF6, 0x44, 0x89, 0x44, 0x24, 0x48, 0x50, 0x48, 0xB8 };

    PVOID pTrampoline = VirtualAlloc(0, 15, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

    allocedOne = pTrampoline;

    if (pTrampoline == nullptr)
        return nullptr;

    lenPathOne = 15;

    if (!VirtualProtect(locationOne, lenPathOne, PAGE_EXECUTE_READWRITE, &oldProtect))
        return nullptr;

    memcpy(originalBytesOne, locationOne, 15);
    memset(locationOne, 0x90, lenPathOne);

    *(byte*)locationOne = 0x50;
    *(WORD*)((QWORD)src + 0x22) = 0xB848;
    *(QWORD*)((QWORD)src + 0x24) = (QWORD)pTrampoline;
    *(WORD*)((QWORD)src + 0x2C) = 0xE0FF;
    *(byte*)((QWORD)src + 0x2E) = 0x58;

    if (!VirtualProtect(locationOne, lenPathOne, oldProtect, &oldProtect))
        return nullptr;

    *(WORD*)pTrampoline = 0xB848;

    *(QWORD*)((QWORD)pTrampoline + 0x2) = (QWORD)var;

    memcpy((void*)((QWORD)pTrampoline + 0xA), stub, sizeof(stub));

    *(QWORD*)((QWORD)pTrampoline + 0x20) = (QWORD)src + 0x2E;

    *(WORD*)((QWORD)pTrampoline + 0x28) = 0xE0FF;

    return locationOne;
}

void* Hook::hook2(void* src, void* dst)
{
    locationTwo = src;

    DWORD oldProtect{};

    byte stub1[] = { 
        0x9D, 0x41, 0x5F, 0x41, 0x5E, 0x41, 0x5D, 0x41, 0x5C, 0x41, 0x5B, 0x41, 0x5A, 0x41, 0x59, 0x41, 0x58, 0x5F, 0x5E, 0x5C, 0x5D, 0x5A, 0x59, 0x5B, 0x58,
        0x48, 0x89, 0x5C, 0x24, 0x10, 0x48, 0x89, 0x74, 0x24, 0x20, 0x55, 0x57, 0x41, 0x56
    };

    PVOID pTrampoline = VirtualAlloc(0, 14, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

    allocedTwo = pTrampoline;

    if (pTrampoline == nullptr)
        return nullptr;

    lenPathTwo = 14;

    if (!VirtualProtect(src, lenPathTwo, PAGE_EXECUTE_READWRITE, &oldProtect))
        return nullptr;

    memcpy(originalBytesTwo, src, lenPathTwo);
    memset(src, 0x90, lenPathTwo);

    *(byte*)src = 0x50;
    *(WORD*)((QWORD)src + 0x1) = 0xB848;
    *(QWORD*)((QWORD)src + 0x3) = (QWORD)pTrampoline;
    *(WORD*)((QWORD)src + 0xB) = 0xE0FF;
    *(byte*)((QWORD)src + 0xD) = 0x58;

    if (!VirtualProtect(src, lenPathTwo, oldProtect, &oldProtect))
        return nullptr;

    byte stub2[] = { 0x53, 0x51, 0x52, 0x55, 0x54, 0x56, 0x57, 0x41, 0x50, 0x41, 0x51, 0x41, 0x52, 0x41, 0x53, 0x41, 0x54, 0x41, 0x55, 0x41, 0x56, 0x41, 0x57, 0x9C };

    memcpy(pTrampoline, stub2, sizeof(stub2));

    *(WORD*)((QWORD)pTrampoline + 0x18) = 0xB848;
    *(QWORD*)((QWORD)pTrampoline + 0x1A) = (QWORD)dst;
    *(WORD*)((QWORD)pTrampoline + 0x22) = 0xD0FF;

    memcpy((void*)((QWORD)pTrampoline + 0x24), stub1, sizeof(stub1));

    *(byte*)((QWORD)pTrampoline + 0x4B) = 0x50;
    *(WORD*)((QWORD)pTrampoline + 0x4C) = 0xB848;
    *(QWORD*)((QWORD)pTrampoline + 0x4E) = (QWORD)src + 0xD;
    *(WORD*)((QWORD)pTrampoline + 0x56) = 0xE0FF;

    return src;
}
#else
void* Hook::hook1(void* src, OUT void* var)
{
    locationOne = (void*)((DWORD)src + 0x1A);

    DWORD oldProtect{};

    byte stub[] = {
        0x8D, 0x4C, 0x24, 0x0C, 0x57, 0xFF, 0x75, 0x10
    };

    PVOID pTrampoline = VirtualAlloc(0, 8, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

    allocedOne = pTrampoline;

    if (pTrampoline == nullptr)
        return nullptr;

    *(WORD*)pTrampoline = 0x3589;
    *(DWORD*)((DWORD)pTrampoline + 2) = (DWORD)var;

    memcpy((void*)((DWORD)pTrampoline + 6), stub, sizeof(stub));

    *(byte*)((DWORD)pTrampoline + 0xE) = 0xE9;

    *(DWORD*)((DWORD)pTrampoline + 0xF) = ((DWORD)(src) + 0x22) - ((DWORD)pTrampoline + 0xE) - 5;

    lenPathOne = 8;

    if(!VirtualProtect(locationOne, lenPathOne, PAGE_EXECUTE_READWRITE, &oldProtect))
        return nullptr;

    memcpy(originalBytesOne, locationOne, 8);
    memset(locationOne, 0x90, lenPathOne);

    *(byte*)locationOne = 0xE9;
    *(DWORD*)((DWORD)src + 0x1B) = (DWORD)pTrampoline - ((DWORD)src + 0x1A) - 5;

    if (!VirtualProtect(locationOne, lenPathOne, oldProtect, &oldProtect))
        return nullptr;

    return (void*)((DWORD)src + 0x1A);
}

void* Hook::hook2(void* src, void* dst)
{
    locationTwo = src;

    DWORD oldProtect{};

    byte stub[] = {
        0x60, 0x9C
    };

    PVOID pTrampoline = VirtualAlloc(0, 5, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

    allocedTwo = pTrampoline;

    if (pTrampoline == nullptr)
        return nullptr;

    memcpy(pTrampoline, stub, sizeof(stub));

    *(byte*)((DWORD)pTrampoline + 2) = 0xE8;

    *(DWORD*)((DWORD)pTrampoline + 3) = (DWORD)dst - ((DWORD)pTrampoline + 2) - 5;

    byte stubTwo[] = {
        0x9D, 0x61, 0x8B, 0xFF, 0x55, 0x8B, 0xEC, 0xE9
    };

    memcpy((void*)((DWORD)pTrampoline + 7), stubTwo, sizeof(stubTwo));

    *(DWORD*)((DWORD)pTrampoline + 0xF) = ((DWORD)src + 5) - ((DWORD)pTrampoline + 0xE) - 5;

    lenPathTwo = 5;

    if (!VirtualProtect(src, lenPathTwo, PAGE_EXECUTE_READWRITE, &oldProtect))
        return nullptr;

    memcpy(originalBytesTwo, locationTwo, 5);

    *(byte*)src = 0xE9;

    *(DWORD*)((DWORD)src + 1) = (DWORD)pTrampoline - (DWORD)src - 5;

    if (!VirtualProtect(src, lenPathTwo, oldProtect, &oldProtect))
        return nullptr;

    return src;
}
#endif
void Hook::Unhook()
{
    DWORD oldProtect{};

    if (lenPathOne != 0) {
        VirtualProtect(locationOne, lenPathOne, PAGE_EXECUTE_READWRITE, &oldProtect);
        memcpy(locationOne, originalBytesOne, lenPathOne);
        VirtualProtect(locationOne, lenPathOne, oldProtect, &oldProtect);
        VirtualFree(allocedOne, 0, MEM_RELEASE);
    }
     
    if (lenPathTwo != 0) {
        VirtualProtect(locationTwo, lenPathTwo, PAGE_EXECUTE_READWRITE, &oldProtect);
        memcpy(locationTwo, originalBytesTwo, lenPathTwo);
        VirtualProtect(locationTwo, lenPathTwo, oldProtect, &oldProtect);
        VirtualFree(allocedTwo, 0, MEM_RELEASE);
    }
}