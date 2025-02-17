#include "Platform/FileSystem.h"

namespace Tange
{
    FileData FileSystem::ReadEntireFile(const std::string& fileName)
    {
        HANDLE hFile = CreateFileA(fileName.c_str(), GENERIC_READ, 0, 
                                   nullptr, OPEN_EXISTING, 0, nullptr);

        // Opening a non-existent file is a runtime assertion, for now.                                
        ASSERT(hFile != INVALID_HANDLE_VALUE);
        if (hFile)
        {
            FileData fileData;
            fileData.Size = GetFileSize(hFile);
            fileData.pData = std::make_shared<char[]>(fileData.Size);

            DWORD bytesRead = 0;
            ReadFile(hFile, fileData.pData.get(), fileData.Size, &bytesRead, nullptr);
            ASSERT(bytesRead == fileData.Size);

            CloseHandle(hFile);
            return fileData;
        }

        return {};
    }
}