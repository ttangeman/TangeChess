#pragma once

#ifdef _WIN32
#define MAIN_ENTRY_POINT() int WINAPI WinMain(HINSTANCE _instance, HINSTANCE _prevInstance, LPSTR _commandLine, int _showCommand)
#else
#error "Only Windows is implemented at this time."
#endif