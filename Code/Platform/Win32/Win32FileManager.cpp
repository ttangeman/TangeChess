#include "Platform/FileManager.h"

namespace Platform
{
    FileManager& FileManager::GetInstance()
    {
        static FileManager instance;
        return instance;
    }

    FileData FileManager::ReadEntireFile(const std::string& fileName) const
    {
        HANDLE hFile = CreateFileA(fileName.c_str(), GENERIC_READ, 0, 
                                   nullptr, OPEN_EXISTING, 0, nullptr);

        // Opening a non-existent file is a runtime assertion, for now.                                
        ASSERT(hFile != INVALID_HANDLE_VALUE);
        if (hFile)
        {
            FileData fileData;
            fileData.Size = Platform::GetFileSize(hFile);
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