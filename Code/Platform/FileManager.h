#pragma once

#include "Core/Common.h"
#include "Platform/PlatformWrapper.h"

namespace Platform
{
    struct FileData
    {
        std::shared_ptr<char[]> pData;
        usize Size;
    };

    // TODO: Is a class even needed for this?
    class FileManager
    {
        public:

        static FileManager& GetInstance();

        // Reads the entire file into the heap and returns the data (a shared pointer and the size).
        FileData ReadEntireFile(const std::string& fileName);

        private:

    };
}