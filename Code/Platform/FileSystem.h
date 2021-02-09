#pragma once

#include "Core/Common.h"
#include "Platform/PlatformWrapper.h"

namespace Tange
{
    struct FileData
    {
        std::shared_ptr<char[]> pData;
        usize Size;
    };

    class FileSystem
    {
    public:
        // Reads the entire file into the heap and returns the data (a shared pointer and the size).
        static FileData ReadEntireFile(const std::string& fileName);
    };
}