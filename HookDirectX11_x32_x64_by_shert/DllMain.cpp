#include <windows.h>
#include <iostream>

#include "DirectX.h"
#include "DebugConsole.h"

Directx direct;

void Render() {
    direct.DrawRectangle();
}

DWORD WINAPI Main(HMODULE hModule) {
#ifdef _DEBUG
    DebugConsole debugConsole{};
    debugConsole.Create();
#endif // _DEBUG

    HRESULT hr = direct.Load();

    if (FAILED(hr))
        direct.Unhook();
    else {
        while (true)
        {
            Sleep(20);
            if (GetAsyncKeyState(VK_END) & 1) {
                direct.Unhook();
                break;
            }
        }
    }

#ifdef _DEBUG
    debugConsole.Delete();
#endif // _DEBUG

    FreeLibraryAndExitThread(hModule, 0);
    return 0;
}

BOOL APIENTRY DllMain( HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        HANDLE threadMain = CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)Main, hModule, 0, nullptr);
        if (threadMain != NULL)
            CloseHandle(threadMain);
        else
            MessageBox(NULL, L"Failed to create main thread ;(", L"Fatal ERROR", MB_OK | MB_ICONERROR | MB_SETFOREGROUND);
    }

    return TRUE;
}

