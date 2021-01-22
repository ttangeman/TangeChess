#include "Win32Wrapper.h"
#include <timeapi.h>
#include "Platform/PlatformManager.h"

namespace Platform
{
    Stopwatch::Stopwatch()
        : m_currentCount(0)
    {
        // Assure that we have 1 ms granularity for timing.
        UINT desiredSchedulerGranularityInMs = 1;
        BOOL granularityResult = timeBeginPeriod(desiredSchedulerGranularityInMs) == TIMERR_NOERROR;
        ASSERT(granularityResult);

        LARGE_INTEGER perfFrequency;
        BOOL queryResult = QueryPerformanceFrequency(&perfFrequency);
        ASSERT(queryResult);

        m_clocksPerSecond = perfFrequency.QuadPart;
    }

    int64 Stopwatch::QueryClock() const
    {
        LARGE_INTEGER clockCount;
        QueryPerformanceCounter(&clockCount);
        return clockCount.QuadPart;
    }

    void Stopwatch::Start()
    {
        m_currentCount = QueryClock();
    }

    float Stopwatch::Stop() const
    {
        float calculatedTime = (float) (QueryClock() - m_currentCount) / 
                               (float) m_clocksPerSecond;
        return calculatedTime;
    }

    PlatformManager& PlatformManager::GetInstance()
    {
        static PlatformManager instance;
        return instance;
    }

    LRESULT CALLBACK Win32MessagePump(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
    {
        LRESULT result;
        switch (message) 
        {
            case WM_DESTROY:
            case WM_CLOSE: 
            {
                PostQuitMessage(0);
                return 0;
            } break;
            
            default: 
            {
                result = DefWindowProcA(window, message, wParam, lParam);
            } break;
        }
        return result;
    }

    static void ProcessWin32KeyboardInput(InputHandler& input, MSG message)
    {
        // NOTE: We can get repeat count for keyboards if we want it
        // See lParam on WM_KEYDOWN or WM_KEYUP.
        LPARAM lParam = message.lParam;
        WPARAM wParam = message.wParam;
        int32 keycode = (int32) wParam;

        //TODO: Handle modifier keys
        bool isDown = message.message == WM_KEYDOWN;
        bool isUp = !isDown;
        
        switch (keycode) 
        {
            case 'W': 
            {
                input.UpdateInputState(InputEvent::MoveForward, isDown, isUp);
            } break;
            
            case 'A': 
            {
                input.UpdateInputState(InputEvent::MoveLeft, isDown, isUp);
            } break;
            
            case 'S': 
            {
                input.UpdateInputState(InputEvent::MoveBackward, isDown, isUp);
            } break;
            
            case 'D': 
            {
                input.UpdateInputState(InputEvent::MoveRight, isDown, isUp);
            } break;
            
            case 'F': 
            {
                input.UpdateInputState(InputEvent::ToggleFullscreen, isDown, isUp);
            } break;
            
            case 'Q': 
            {
                input.UpdateInputState(InputEvent::SystemQuit, isDown, isUp);
            } break;
            
            case VK_ESCAPE: 
            {
                input.UpdateInputState(InputEvent::ToggleMainMenu, isDown, isUp);
            } break;
        }
    }

    void PlatformManager::HandleSystemMessages() 
    {
        MSG message;
        
        auto& input = InputHandler::GetInstance();

        if (PeekMessage(&message, 0, 0, 0, PM_REMOVE)) 
        {
            switch (message.message) 
            {
                case WM_KEYDOWN:
                case WM_KEYUP: 
                {
                    ProcessWin32KeyboardInput(input, message);
                } break;
                
                // NOTE: We can capture double clicks and modifiers on clicks
                // if we want to. Check "Responding to Mouse Clicks" on MSDN for
                // more info.
                case WM_LBUTTONDOWN: 
                {
                    input.UpdateInputState(InputEvent::LeftClick, true, false);
                } break;
                
                case WM_RBUTTONDOWN: 
                {
                    input.UpdateInputState(InputEvent::RightClick, true, false);
                } break;
                
                case WM_MBUTTONDOWN: 
                {
                    input.UpdateInputState(InputEvent::MiddleClick, true, false);
                } break;
                
                case WM_LBUTTONUP: 
                {
                    input.UpdateInputState(InputEvent::LeftClick, false, true);
                } break;
                
                case WM_RBUTTONUP: 
                {
                    input.UpdateInputState(InputEvent::RightClick, false, true);
                } break;
                
                case WM_MBUTTONUP: 
                {
                    input.UpdateInputState(InputEvent::MiddleClick, false, true);
                } break;
                
                case WM_MOUSEMOVE: 
                {
                    input.UpdateMousePosition();
                } break;
                
                case WM_QUIT: 
                {
                    ForceQuit();
                } break;
                
                default: 
                {
                    TranslateMessage(&message);
                    DispatchMessage(&message);
                } break;
            }
        }
    }

    bool PlatformManager::InitializeAndCreateWindow(const char* pWindowTitle, int32 windowWidth, 
                                                    int32 windowHeight, bool showCursor, 
                                                    bool useWindowBorders)
    {
        HINSTANCE hInstance = GetModuleHandle(0);
        
        WNDCLASSA windowClass = {};
        windowClass.lpfnWndProc = Win32MessagePump;
        windowClass.hInstance = hInstance;
        windowClass.lpszClassName = "Win32Class";
        windowClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
        RegisterClassA(&windowClass);
        
        m_hWindow = CreateWindowExA(0, windowClass.lpszClassName, pWindowTitle, 
                                   WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 
                                   windowWidth, windowHeight, 0, 0, hInstance, 0);
        
        if (m_hWindow) 
        {
            // Initialize input handler.
            auto& inputHandler = InputHandler::GetInstance();
            inputHandler.Initialize(m_hWindow);

            m_shouldQuit = false;
            
            ShowWindow(m_hWindow, SW_SHOWNORMAL);
            ShowCursor(showCursor);
            
            if (!useWindowBorders)
            {
                // Remove the Windows window border
                LONG windowStyle = GetWindowLong(m_hWindow, GWL_STYLE);
                windowStyle &= ~(WS_CAPTION | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU);
                SetWindowLongPtr(m_hWindow, GWL_STYLE, windowStyle);
                
                LONG exWindowStyle = GetWindowLong(m_hWindow, GWL_EXSTYLE);
                exWindowStyle &= ~(WS_EX_DLGMODALFRAME | WS_EX_CLIENTEDGE | WS_EX_STATICEDGE);
                SetWindowLongPtr(m_hWindow, GWL_EXSTYLE, exWindowStyle);
                
                SetWindowPos(m_hWindow, nullptr, 0, 0, 0, 0,
                             SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE |
                             SWP_NOZORDER | SWP_NOOWNERZORDER);
            }
            return true;
        }
        return false;
    }

    void PlatformManager::Shutdown()
    {
        DestroyWindow(m_hWindow);
    }

    bool PlatformManager::ShouldQuit() const
    {
        return m_shouldQuit;
    }

    void PlatformManager::ForceQuit()
    {
        m_shouldQuit = true;
    }

    Vec2 PlatformManager::GetRenderDimensions() const
    {
        Vec2 result;
        RECT windowRect;
        GetClientRect(m_hWindow, &windowRect);
        
        result.Width = (float) (windowRect.right - windowRect.left);
        result.Height = (float) (windowRect.bottom - windowRect.top);
        
        return result;
    }

    WindowHandle PlatformManager::GetWindow() const
    {
        return m_hWindow;
    }
}