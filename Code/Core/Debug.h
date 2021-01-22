#pragma once

#define DEBUG_BUILD 1

#if DEBUG_BUILD
#define ASSERTIONS_ENABLED 1
#endif

#if ASSERTIONS_ENABLED
#define DEBUG_CRASH() if (1) { int* _x = 0; *_x = 0;}
#define INVALID_CODE_PATH() DEBUG_CRASH();
#define NOT_YET_IMPLEMENTED() DEBUG_CRASH();
#define ASSERT(expr) \
if (expr) { \
} else { \
DEBUG_CRASH(); \
}
#else
#define DEBUG_CRASH()
#define ASSERT(expr)
#define INVALID_CODE_PATH()
#define NOT_YET_IMPLEMENTED()
#endif