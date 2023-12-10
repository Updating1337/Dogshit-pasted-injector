#include "StdAfx.h"
#include "Textures.h"

void Textures::Init(ID3D11Device* pDevice)
{
	pump0 = nullptr;

	tCharacter == nullptr;
	tBanner == nullptr;
	tAvatar == nullptr;
	tValorant == nullptr;
	tRust == nullptr;
	tRust == nullptr;
	tCSGO == nullptr;
	tCS2 == nullptr;
	tAmongUs == nullptr;
	tMinecraft == nullptr;

	tSpoofer = nullptr;
	tDayz = nullptr;

	D3DX11CreateShaderResourceViewFromMemory(pDevice, byteCharacter, sizeof(byteCharacter), &info0, pump0, &tCharacter, 0);
	D3DX11CreateShaderResourceViewFromMemory(pDevice, byteBanner, sizeof(byteBanner), &info0, pump0, &tBanner, 0);
	D3DX11CreateShaderResourceViewFromMemory(pDevice, byteAvatar, sizeof(byteAvatar), &info0, pump0, &tAvatar, 0);

	D3DX11CreateShaderResourceViewFromMemory(pDevice, byteValorant, sizeof(byteValorant), &info0, pump0, &tValorant, 0);
	D3DX11CreateShaderResourceViewFromMemory(pDevice, byteRust, sizeof(byteRust), &info0, pump0, &tRust, 0);
	D3DX11CreateShaderResourceViewFromMemory(pDevice, byteCSGO, sizeof(byteCSGO), &info0, pump0, &tCSGO, 0);
	D3DX11CreateShaderResourceViewFromMemory(pDevice, byteCS2, sizeof(byteCS2), &info0, pump0, &tCS2, 0);
	D3DX11CreateShaderResourceViewFromMemory(pDevice, byteAmongUs, sizeof(byteAmongUs), &info0, pump0, &tAmongUs, 0);
	D3DX11CreateShaderResourceViewFromMemory(pDevice, byteMinecraft, sizeof(byteMinecraft), &info0, pump0, &tMinecraft, 0);

	D3DX11CreateShaderResourceViewFromMemory(pDevice, byteSpoofer, sizeof(byteSpoofer), &info0, pump0, &tSpoofer, 0);
	D3DX11CreateShaderResourceViewFromMemory(pDevice, byteDayZ, sizeof(byteDayZ), &info0, pump0, &tDayz, 0);

}

void Textures::Unload()
{
	tMinecraft->Release();
	tAmongUs->Release();
	tCS2->Release();
	tCSGO->Release();
	tRust->Release();
	tValorant->Release();

	tAvatar->Release();
	tBanner->Release();
	tCharacter->Release();

	tDayz->Release();
	tSpoofer->Release();
}
