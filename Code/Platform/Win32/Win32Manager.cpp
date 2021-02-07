#include "Win32Wrapper.h"
#include <timeapi.h>
#include "Platform/PlatformManager.h"

namespace Tange
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

    PlatformManager PlatformManager::s_instance;

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

            case WM_SIZE:
            {
                Vec2 currentDim = PlatformManager::GetRenderDimensions();
                Vec2 desiredDim = PlatformManager::RecomputeRenderDimensions();

                EventManager::Dispatch<WindowResized>(WindowResized(desiredDim.Width, desiredDim.Height, 
                                                                    currentDim.Width, currentDim.Height));

            } break;
            
            default: 
            {
                result = DefWindowProcA(window, message, wParam, lParam);
            } break;
        }
        return result;
    }

    static void ProcessWin32KeyboardInput(MSG message)
    {
        // NOTE: We can get repeat count for keyboards if we want it
        // See lParam on WM_KEYDOWN or WM_KEYUP.
        LPARAM lParam = message.lParam;
        WPARAM wParam = message.wParam;
        int32 keycode = (int32) wParam;

        //TODO: Handle modifier keys
        bool isDown = message.message == WM_KEYDOWN;
        bool isUp = !isDown;
        
        if (isDown)
        {
            EventManager::Dispatch<KeyPressed>(KeyPressed((InputEvent)keycode));
        }

        if (isUp)
        {
            EventManager::Dispatch<KeyReleased>(KeyReleased((InputEvent)keycode));
        }
    }

    Vec2 PlatformManager::CalculateMousePosition()
    {
        POINT cursorP;
        // Gets the mouse position relative to the screen.
        GetCursorPos(&cursorP);
        // This maps the mouse position into the window's dimensions.
        ScreenToClient(s_instance.m_hWindow, &cursorP);

        RECT clientRect;
        GetClientRect(s_instance.m_hWindow, &clientRect);
        auto clientHeight = clientRect.bottom - clientRect.top;

        // Flip the Y to bottom-up instead of top-down.
        cursorP.y = ABS_VALUE(cursorP.y - clientHeight);
        return Vec2(cursorP.x, cursorP.y);
    }

    void PlatformManager::DispatchSystemMessages() 
    {
        MSG message;

        if (PeekMessage(&message, 0, 0, 0, PM_REMOVE)) 
        {
            switch (message.message) 
            {
                case WM_KEYDOWN:
                case WM_KEYUP: 
                {
                    ProcessWin32KeyboardInput(message);
                } break;
                
                // NOTE: We can capture double clicks and modifiers on clicks
                // if we want to. Check "Responding to Mouse Clicks" on MSDN for
                // more info.
                case WM_LBUTTONDOWN: 
                {
                    Vec2 position = CalculateMousePosition();
                    EventManager::Dispatch<MouseClicked>(MouseClicked(InputEvent::LeftClick, position));
                } break;
                
                case WM_RBUTTONDOWN: 
                {
                    Vec2 position = CalculateMousePosition();
                    EventManager::Dispatch<MouseClicked>(MouseClicked(InputEvent::RightClick, position));
                } break;
                
                case WM_MBUTTONDOWN: 
                {
                    Vec2 position = CalculateMousePosition();
                    EventManager::Dispatch<MouseClicked>(MouseClicked(InputEvent::MiddleClick, position));
                } break;
                
                case WM_LBUTTONUP: 
                {
                    Vec2 position = CalculateMousePosition();
                    EventManager::Dispatch<MouseReleased>(MouseReleased(InputEvent::LeftClick, position));
                } break;
                
                case WM_RBUTTONUP: 
                {
                    Vec2 position = CalculateMousePosition();
                    EventManager::Dispatch<MouseReleased>(MouseReleased(InputEvent::RightClick, position));
                } break;
                
                case WM_MBUTTONUP: 
                {
                    Vec2 position = CalculateMousePosition();
                    EventManager::Dispatch<MouseReleased>(MouseReleased(InputEvent::MiddleClick, position));
                } break;

                case WM_MOUSEMOVE:
                {
                    Vec2 position = CalculateMousePosition();
                    EventManager::Dispatch<MouseMoved>(MouseMoved(position));
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
        EventManager::RegisterEvent<KeyPressed>();
        EventManager::RegisterEvent<KeyReleased>();
        EventManager::RegisterEvent<MouseClicked>();
        EventManager::RegisterEvent<MouseReleased>();
        EventManager::RegisterEvent<MouseMoved>();
        EventManager::RegisterEvent<WindowResized>();

        HINSTANCE hInstance = GetModuleHandle(0);
        
        WNDCLASSA windowClass = {};
        windowClass.lpfnWndProc = Win32MessagePump;
        windowClass.hInstance = hInstance;
        windowClass.lpszClassName = "Win32Class";
        windowClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
        RegisterClassA(&windowClass);
        
        s_instance.m_hWindow = CreateWindowExA(0, windowClass.lpszClassName, pWindowTitle, 
                                               WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 
                                               windowWidth, windowHeight, 0, 0, hInstance, 0);
        
        if (s_instance.m_hWindow) 
        {
            s_instance.m_shouldQuit = false;
            
            RecomputeRenderDimensions();
            RecomputeWindowDimensions();

            ShowWindow(s_instance.m_hWindow, SW_SHOWNORMAL);
            ShowCursor(showCursor);
            
            if (!useWindowBorders)
            {
                // Remove the Windows window border
                LONG windowStyle = GetWindowLong(s_instance.m_hWindow, GWL_STYLE);
                windowStyle &= ~(WS_CAPTION | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU);
                SetWindowLongPtr(s_instance.m_hWindow, GWL_STYLE, windowStyle);
                
                LONG exWindowStyle = GetWindowLong(s_instance.m_hWindow, GWL_EXSTYLE);
                exWindowStyle &= ~(WS_EX_DLGMODALFRAME | WS_EX_CLIENTEDGE | WS_EX_STATICEDGE);
                SetWindowLongPtr(s_instance.m_hWindow, GWL_EXSTYLE, exWindowStyle);
                
                SetWindowPos(s_instance.m_hWindow, nullptr, 0, 0, 0, 0,
                             SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE |
                             SWP_NOZORDER | SWP_NOOWNERZORDER);
            }
            return true;
        }
        return false;
    }

    void PlatformManager::Shutdown()
    {
        DestroyWindow(s_instance.m_hWindow);
    }

    bool PlatformManager::ShouldQuit()
    {
        return s_instance.m_shouldQuit;
    }

    void PlatformManager::ForceQuit()
    {
       s_instance.m_shouldQuit = true;
    }

    Vec2 PlatformManager::RecomputeRenderDimensions()
    {
        // Get the size of the windows actual draw dimensions (i.e. no
        // window borders).
        RECT windowRect;
        GetClientRect(s_instance.m_hWindow, &windowRect);
        s_instance.m_renderDimensions.Width = (float) (windowRect.right - windowRect.left);
        s_instance.m_renderDimensions.Height = (float) (windowRect.bottom - windowRect.top);
        return s_instance.m_renderDimensions;
    }

    Vec2 PlatformManager::GetRenderDimensions()
    {
        return s_instance.m_renderDimensions;
    }

    Vec2 PlatformManager::RecomputeWindowDimensions()
    {
        RECT windowRect;
        GetWindowRect(s_instance.m_hWindow, &windowRect);
        s_instance.m_windowDimensions.Width = (float) (windowRect.right - windowRect.left);
        s_instance.m_windowDimensions.Height = (float) (windowRect.bottom - windowRect.top);
        return s_instance.m_windowDimensions;
    }

    Vec2 PlatformManager::GetWindowDimensions()
    {
        return s_instance.m_windowDimensions;
    }

    WindowHandle PlatformManager::GetWindow()
    {
        return s_instance.m_hWindow;
    }
}