#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "Core/Common.h"

// NOTE: The wrapper must define a type alias to the appropiate
// type for a window handle and file handle!
typedef HWND WindowHandle;
typedef HANDLE FileHandle;

namespace Platform 
{
    void* AllocateMemory(usize size) 
    {
        void* pResult = VirtualAlloc(nullptr, size, 
                                     MEM_COMMIT|MEM_RESERVE, 
                                     PAGE_READWRITE);
        Assert(pResult);
        return pResult;
    }
    
    void* AllocateMemory(usize size, void* pBaseAddress)
     {
        void* pResult = VirtualAlloc(pBaseAddress, size, 
                                    MEM_COMMIT|MEM_RESERVE, 
                                    PAGE_READWRITE);
        Assert(pResult);
        return pResult;
    }
    
    void Copy(void* pDestination, const void* pSource, usize size) 
    {
        CopyMemory(pDestination, pSource, size);
    }
    
    void Zero(void* pDestination, usize size) 
    {
        ZeroMemory(pDestination, size);
    }
    
    void FreeMemory(void* pMemory) 
    {
        if (pMemory)
            VirtualFree(pMemory, 0, MEM_RELEASE);
    }
    
    uint32 GetAllocationGranularity() 
    {
        SYSTEM_INFO info = {};
        GetSystemInfo(&info);
        return info.dwPageSize;
    }

    uint64 GetFileSize(FileHandle hFile)
    {
        LARGE_INTEGER fileSize;
        ::GetFileSizeEx(hFile, &fileSize);
        return (uint64)fileSize.QuadPart;
    }

    uint32 CreateThread(void* pEntryPoint, void* args, usize desiredStackSize) 
    {
        DWORD threadId;
        HANDLE threadHandle = ::CreateThread(nullptr, desiredStackSize, 
                                             (LPTHREAD_START_ROUTINE)pEntryPoint,
                                             args, 0, (LPDWORD) &threadId);
        return threadId;
    }
    
    uint32 CreateSuspendedThread(void* pEntryPoint, void* args, usize desiredStackSize) 
    {
        DWORD threadId;
        HANDLE threadHandle = ::CreateThread(nullptr, desiredStackSize, 
                                             (LPTHREAD_START_ROUTINE)pEntryPoint,
                                             args, CREATE_SUSPENDED, (LPDWORD) &threadId);
        return threadId;
    }
    
    void WaitForThread(uint32 id) 
    {
        HANDLE handle = OpenThread(SYNCHRONIZE, false, id);
        DWORD result = WaitForSingleObject(handle, INFINITE);
        CloseHandle(handle);
    }
    
    void ResumeThread(uint32 id) 
    {
        HANDLE handle = OpenThread(THREAD_SUSPEND_RESUME, false, id);
        DWORD result = ::ResumeThread(handle);
        CloseHandle(handle);
    }
    
    void SleepMs(uint64 sleepTime) 
    {
        Sleep((DWORD)sleepTime);
    }
};