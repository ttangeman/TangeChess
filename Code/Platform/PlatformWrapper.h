#include "Core/Common.h"

#ifdef _WIN32
#include "Platform/Win32/Win32Wrapper.h"
#else
#error "Only Windows is implemented at this time."
#endif