#include "StdAfx.h"
#include "DirectX.h"
#include "Window.h"

bool DirectX::Init()
{
    if (!CreateDeviceD3D(Window::hWnd))
    {
        CleanupDeviceD3D();
        Window::Shutdown();
        return false;
    }
    return true;
}

bool DirectX::CreateDeviceD3D(HWND hWnd)
{
    // Setup swap chain
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createDeviceFlags = 0;
    //createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
    if (D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &m_pSwapChain, &m_pDevice, &featureLevel, &m_pDeviceContext) != S_OK)
        return false;

    CreateRenderTarget();
    return true;
}

void DirectX::Present()
{
    const float clear_color_with_alpha[4] = { 0 };
    m_pDeviceContext->OMSetRenderTargets(1, &m_pMainRenderTargetView, NULL);
    m_pDeviceContext->ClearRenderTargetView(m_pMainRenderTargetView, clear_color_with_alpha);
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    m_pSwapChain->Present(bSyncInterval ? 1 : 0, 0);
}

void DirectX::CreateRenderTarget()
{
    ID3D11Texture2D* pBackBuffer;
    m_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    m_pDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_pMainRenderTargetView);
    pBackBuffer->Release();
}

void DirectX::CleanupDeviceD3D()
{
    CleanupRenderTarget();
    if (m_pSwapChain) { m_pSwapChain->Release(); m_pSwapChain = NULL; }
    if (m_pDeviceContext) { m_pDeviceContext->Release(); m_pDeviceContext = NULL; }
    if (m_pDevice) { m_pDevice->Release(); m_pDevice = NULL; }
}

void DirectX::CleanupRenderTarget()
{
    if (m_pMainRenderTargetView) { m_pMainRenderTargetView->Release(); m_pMainRenderTargetView = NULL; }
}
