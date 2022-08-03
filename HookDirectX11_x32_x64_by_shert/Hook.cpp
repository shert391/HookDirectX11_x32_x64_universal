#include <windows.h>
#include "Hooks.h"

typedef __int64 QWORD;

#ifdef _WIN64
void* Hook::hook1(void* src, OUT void* var)
{
    DWORD oldProtect{};

    byte stub[] = { 0x48, 0x89, 0x08, 0x58, 0x48, 0x33, 0xC4, 0x48, 0x89, 0x45, 0x60, 0x45, 0x33, 0xF6, 0x44, 0x89, 0x44, 0x24, 0x48, 0x50, 0x48, 0xB8 };

    PVOID pTrampoline = VirtualAlloc(0, 20, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

    if (pTrampoline == nullptr)
        return nullptr;

    if (!VirtualProtect((void*)((QWORD)src + 0x21), 15, PAGE_EXECUTE_READWRITE, &oldProtect))
        return nullptr;

    memset((void*)((QWORD)src + 0x21), 0x90, 15);

    *(byte*)((QWORD)src + 0x21) = 0x50;
    *(WORD*)((QWORD)src + 0x22) = 0xB848;
    *(QWORD*)((QWORD)src + 0x24) = (QWORD)pTrampoline;
    *(WORD*)((QWORD)src + 0x2C) = 0xE0FF;
    *(byte*)((QWORD)src + 0x2E) = 0x58;

    if (!VirtualProtect((void*)((QWORD)src + 0x21), 15, oldProtect, &oldProtect))
        return nullptr;

    *(WORD*)pTrampoline = 0xB848;

    *(QWORD*)((QWORD)pTrampoline + 0x2) = (QWORD)var;

    memcpy((void*)((QWORD)pTrampoline + 0xA), stub, sizeof(stub));

    *(QWORD*)((QWORD)pTrampoline + 0x20) = (QWORD)src + 0x2E;

    *(WORD*)((QWORD)pTrampoline + 0x28) = 0xE0FF;

    return (void*)((QWORD)src + 0x21);
}

void* Hook::hook2(void* src, void* dst)
{
    DWORD oldProtect{};

    byte stub1[] = { 
        0x9D, 0x41, 0x5F, 0x41, 0x5E, 0x41, 0x5D, 0x41, 0x5C, 0x41, 0x5B, 0x41, 0x5A, 0x41, 0x59, 0x41, 0x58, 0x5F, 0x5E, 0x5C, 0x5D, 0x5A, 0x59, 0x5B, 0x58,
        0x48, 0x89, 0x5C, 0x24, 0x10, 0x48, 0x89, 0x74, 0x24, 0x20, 0x55, 0x57, 0x41, 0x56
    };

    PVOID pTrampoline = VirtualAlloc(0, 20, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

    if (pTrampoline == nullptr)
        return nullptr;

    if (!VirtualProtect(src, 14, PAGE_EXECUTE_READWRITE, &oldProtect))
        return nullptr;

    memset(src, 0x90, 14);

    *(byte*)src = 0x50;
    *(WORD*)((QWORD)src + 0x1) = 0xB848;
    *(QWORD*)((QWORD)src + 0x3) = (QWORD)pTrampoline;
    *(WORD*)((QWORD)src + 0xB) = 0xE0FF;
    *(byte*)((QWORD)src + 0xD) = 0x58;

    if (!VirtualProtect(src, 14, oldProtect, &oldProtect))
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
    DWORD oldProtect{};

    byte stub[] = {
        0x8D, 0x4C, 0x24, 0x0C, 0x57, 0xFF, 0x75, 0x10
    };

    PVOID pTrampoline = VirtualAlloc(0, 20, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

    if (pTrampoline == nullptr)
        return nullptr;

    *(WORD*)pTrampoline = 0x3589;
    *(DWORD*)((DWORD)pTrampoline + 2) = (DWORD)var;

    memcpy((void*)((DWORD)pTrampoline + 6), stub, sizeof(stub));

    *(byte*)((DWORD)pTrampoline + 0xE) = 0xE9;

    *(DWORD*)((DWORD)pTrampoline + 0xF) = ((DWORD)(src) + 0x22) - ((DWORD)pTrampoline + 0xE) - 5;

    if(!VirtualProtect((void*)((DWORD)src + 0x1A), 0x8, PAGE_EXECUTE_READWRITE, &oldProtect))
        return nullptr;

    memset((void*)((DWORD)src + 0x1A), 0x90, 0x8);

    *(byte*)((DWORD)src + 0x1A) = 0xE9;
    *(DWORD*)((DWORD)src + 0x1B) = (DWORD)pTrampoline - ((DWORD)src + 0x1A) - 5;

    if (!VirtualProtect((void*)((DWORD)src + 0x1A), 0x8, oldProtect, &oldProtect))
        return nullptr;

    return (void*)((DWORD)src + 0x1A);
}

void* Hook::hook2(void* src, void* dst)
{
    DWORD oldProtect{};

    byte stub[] = {
        0x60, 0x9C
    };

    PVOID pTrampoline = VirtualAlloc(0, 20, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

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

    if (!VirtualProtect(src, 0x5, PAGE_EXECUTE_READWRITE, &oldProtect))
        return nullptr;

    *(byte*)src = 0xE9;

    *(DWORD*)((DWORD)src + 1) = (DWORD)pTrampoline - (DWORD)src - 5;

    if (!VirtualProtect(src, 0x5, oldProtect, &oldProtect))
        return nullptr;

    return src;
}
#endif