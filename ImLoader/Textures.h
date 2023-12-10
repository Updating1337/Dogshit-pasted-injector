#pragma once

#include <d3d11.h>
#include <D3DX11tex.h>

#include <windows.h>
#include <string> 

#include "Thirdparty/Dear ImGui/imgui.h"

#include "Resources/Textures/Avatar.h"
#include "Resources/Textures/Banner.h"
#include "Resources/Textures/Character.h"

#include "Resources/Textures/Valorant.h"
#include "Resources/Textures/Rush.h"
#include "Resources/Textures/CSGO.h"
#include "Resources/Textures/CS2.h"
#include "Resources/Textures/AmongUs.h"
#include "Resources/Textures/Minecraft.h"

#include "Resources/Textures/Spoofer.h"
#include "Resources/Textures/Dayz.h"


namespace Textures
{
	void Init(ID3D11Device* pDevice);
	void Unload();

	inline ID3D11ShaderResourceView* tBanner;
	inline ID3D11ShaderResourceView* tAvatar;
	inline ID3D11ShaderResourceView* tCharacter;

	inline ID3D11ShaderResourceView* tValorant;
	inline ID3D11ShaderResourceView* tRust;
	inline ID3D11ShaderResourceView* tCSGO;
	inline ID3D11ShaderResourceView* tCS2;
	inline ID3D11ShaderResourceView* tAmongUs;
	inline ID3D11ShaderResourceView* tMinecraft;

	inline ID3D11ShaderResourceView* tSpoofer;
	inline ID3D11ShaderResourceView* tDayz;


	inline D3DX11_IMAGE_LOAD_INFO info0;
	inline ID3DX11ThreadPump* pump0;
};