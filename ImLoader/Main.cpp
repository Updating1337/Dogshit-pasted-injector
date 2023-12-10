#include "StdAfx.h"

#include "DirectX.h"
#include "Window.h"
#include "Gui.h"

#include "ThemidaSDK/Include/C/ThemidaSDK.h"
//#include "../protection/antiDbg.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
   // hide_thread(LI_FN(GetCurrentThread).get()());
   // LI_FN(CreateThread).get()(NULL, NULL, (LPTHREAD_START_ROUTINE)security_loop, NULL, NULL, NULL);
    
    Window::CreateWindowAndClass();
    DirectX::Init();
    Textures::Init(DirectX::m_pDevice);
    Window::Setup();
    Gui::Init(Window::hWnd, DirectX::m_pDevice, DirectX::m_pDeviceContext); // Mive into DirectX or use g_Globals
    
    while (Window::bIsRunning)
    {
        MSG msg;
        while (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                Window::bIsRunning = false;
        }

        Gui::Render();
        DirectX::Present();
    }

    Gui::Unload();
    Textures::Unload();
    DirectX::CleanupDeviceD3D();
    Window::Shutdown();
    
    return 0;
}
