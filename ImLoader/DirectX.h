#pragma once

namespace DirectX
{
	// Initialize Direct3D
	bool Init();
	bool CreateDeviceD3D(HWND hWnd);
	void Present();
	void CreateRenderTarget();
	void CleanupDeviceD3D();
	void CleanupRenderTarget();

	inline bool bSyncInterval = true;

	inline ID3D11Device* m_pDevice = nullptr;
	inline ID3D11DeviceContext* m_pDeviceContext = nullptr;
	inline IDXGISwapChain* m_pSwapChain = nullptr;
	inline ID3D11RenderTargetView* m_pMainRenderTargetView = nullptr;
}