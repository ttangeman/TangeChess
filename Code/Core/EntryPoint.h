#pragma once

#ifdef _WIN32
#define AppEntryPoint() int WINAPI WinMain(HINSTANCE _instance, HINSTANCE _prevInstance, LPSTR _commandLine, int _showCommand)
#else
#error "Only Windows is implemented at this time."
#endif