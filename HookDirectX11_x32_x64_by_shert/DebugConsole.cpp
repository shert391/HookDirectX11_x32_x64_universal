#include <iostream>
#include <windows.h>

#include "DebugConsole.h"

void DebugConsole::Create()
{
    AllocConsole();
    freopen_s(&fp, "CONOUT$", "w", stdout);
}

void DebugConsole::Delete()
{
    fclose(fp);
    FreeConsole();
}
