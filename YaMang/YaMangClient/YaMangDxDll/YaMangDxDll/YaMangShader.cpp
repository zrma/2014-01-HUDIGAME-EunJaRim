#include "stdafx.h"
#include "yaMangDxDll.h"
#include "GlobalVar.h"

YAMANGDXDLL_API HRESULT ShaderImport( LPCTSTR effectFile )
{
	DWORD dwShaderFlags = D3DXFX_NOT_CLONEABLE | D3DXSHADER_DEBUG | D3DXSHADER_NO_PRESHADER;

	HRESULT hr = S_OK;
	hr = D3DXCreateEffectFromFile( g_D3dDevice, effectFile, NULL, NULL, dwShaderFlags, NULL, &g_Effect, NULL );

	if ( FAILED( hr ) )
	{
		MessageBox( NULL, L"Effect Load Failed", effectFile , MB_OK );
	 	return E_FAIL;
	}

	return S_OK;
}

YAMANGDXDLL_API HRESULT MeshTextureImport( LPCTSTR meshTexture )
{
	if ( FAILED( D3DXCreateTextureFromFile( g_D3dDevice, meshTexture, &g_MeshTexture ) ) )
	{
		MessageBox( NULL, L"Mesh Texture Load Failed", meshTexture, MB_OK );
		return E_FAIL;
	}

	if ( g_Effect )
	{
		g_Effect->SetTexture( "tex0", g_MeshTexture );
	}
	return S_OK;
}