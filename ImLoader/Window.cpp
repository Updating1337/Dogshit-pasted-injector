#include "StdAfx.h"
#include "Window.h"
#include "DirectX.h"

void Window::CreateWindowAndClass()
{
    wc.cbSize = sizeof(WNDCLASSEXW);
    wc.style = CS_CLASSDC;
    wc.lpfnWndProc = Window::WindowProc;
    wc.cbClsExtra = NULL;
    wc.cbWndExtra = NULL;
    wc.hInstance = nullptr;
    wc.hIcon = LoadIcon(0, IDI_APPLICATION);
    wc.hCursor = LoadCursor(0, IDC_ARROW);
    wc.hbrBackground = nullptr;
    wc.lpszMenuName = reinterpret_cast<LPCWSTR>(WINDOW_TITE));
    wc.lpszClassName = reinterpret_cast<LPCWSTR>(("Class01"));
    wc.hIconSm = LoadIcon(0, IDI_APPLICATION);

    RegisterClassExW(&wc);
    hWnd = CreateWindowExW(NULL, wc.lpszClassName, wc.lpszMenuName, WS_POPUP, (GetSystemMetrics(SM_CXSCREEN) / 2) - (WindowSize.x / 2), (GetSystemMetrics(SM_CYSCREEN) / 2) - (WindowSize.y / 2), WindowSize.x, WindowSize.y, 0, 0, 0, 0);

    SetWindowLongA(hWnd, GWL_EXSTYLE, GetWindowLong(hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
    SetLayeredWindowAttributes(hWnd, RGB(0, 0, 0), 255, LWA_ALPHA);

    MARGINS margins = { -1 };
    DwmExtendFrameIntoClientArea(hWnd, &margins);

    POINT mouse;
    rc = { 0 };
    GetWindowRect(hWnd, &rc);
}

void Window::Setup()
{
    ::ShowWindow(hWnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hWnd);
}

void Window::Shutdown()
{
    ::DestroyWindow(hWnd);
    ::UnregisterClassA(reinterpret_cast<LPCSTR>(wc.lpszClassName), wc.hInstance);
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam); // Forward declare message handler from imgui_impl_win32.cpp
LRESULT CALLBACK Window::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
        return true;

    switch (message)
    {
        case WM_SIZE:
        {
            if (DirectX::m_pDevice != NULL && wParam != SIZE_MINIMIZED)
            {
                DirectX::CleanupRenderTarget();
                DirectX::m_pSwapChain->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
                DirectX::CreateRenderTarget();
            }
            return 0;
        }

        case WM_SYSCOMMAND:
        {
            if ((wParam & 0xFFF0) == SC_KEYMENU) // Disable ALT application menu
                return 0L;
        }

        case WM_DESTROY:
        {
            ::PostQuitMessage(0);
            return 0L;
        }

        case WM_LBUTTONDOWN:
        {
            Window::GuiPosition = MAKEPOINTS(lParam);
            return 0L;
        }

        case WM_MOUSEMOVE:
        {
            if (wParam == MK_LBUTTON)
            {
                const auto points = MAKEPOINTS(lParam);
                auto rect = ::RECT{ };

                GetWindowRect(hWnd, &rect);

                rect.left += points.x - Window::GuiPosition.x;
                rect.top += points.y - Window::GuiPosition.y;

                /*
                if (Window::GuiPosition.x >= 0 &&
                    Window::GuiPosition.x >= 0 && Window::GuiPosition.x <= (Window::WindowSize.x - Gui::CloseButtonWidth) &&
                    Window::GuiPosition.y >= 0 && Window::GuiPosition.y <= Gui::TitleBarHeight)
                */
                if (GImGui->MovingWindow)
                {
                    SetWindowPos(
                        hWnd,
                        HWND_TOPMOST,
                        rect.left, rect.top,
                        0, 0,
                        SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOZORDER
                    );
                }
            }

            return 0L;
        }
    }

    return ::DefWindowProc(hWnd, message, wParam, lParam);
}