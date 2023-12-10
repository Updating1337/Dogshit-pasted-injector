#pragma once

namespace Gui
{
	void	Init( HWND hWnd, ID3D11Device* device, ID3D11DeviceContext* device_context );
	void	Customize( void );
	void	Render( void );
	void	Unload( void );

	void	DrawLogo(ImVec2 size);
	void	DrawCloseButton();
	void	DrawInjectButton();
	void	DrawSignInButton();
	
	inline float fShadowSize = 10;
	inline float fCharacterWidth = 200;

	inline ImFont* pFont, * FontAwesome, * FontAwesomeBrands, * BiggerFont, * LogoFont;

	inline ImVec2 LogoSize = ImVec2(40, 40);
	inline int SelectedGame = 2;
}

// Note: Don't use this as an account system, this is created only for testing, and demo

static class CStatus
{
public:
	bool	m_bIsLogged		= false;
	bool	m_bIsLogging	= false;
	bool	m_bIsInjecting	= false;
	bool	m_bIsInjected	= false;
} g_Status;