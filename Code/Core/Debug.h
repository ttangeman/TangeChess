#pragma once

#define DEBUG_BUILD 1

#if DEBUG_BUILD
#define ASSERTIONS_ENABLED 1
#endif

#if ASSERTIONS_ENABLED
#define DebugCrash() if (1) { int* _x = 0; *_x = 0;}
#define InvalidCodePath() DebugCrash();
#define NotYetImplemented() DebugCrash();
#define Assert(expr) \
if (expr) { \
} else { \
DebugCrash(); \
}
#else
#define DebugCrash()
#define Assert(expr)
#define InvalidCodePath()
#define NotYetImplemented()
#endif