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


//////////////////////////////////////////////////////////////////////////
// 이거슨 텍스쳐 쪽 코드가 아니므니다 - 옮겨야 됨 ㅠㅠ 어쩌다보니 일단 여기
//////////////////////////////////////////////////////////////////////////
YAMANGDXDLL_API void DrawBillboardByTexture( int id )
{
	if ( id <= 0 || id >= g_MeshTextureSize )
	{
		return;
	}

	g_D3dDevice->SetVertexShader( NULL );
	g_IsEffectReady = false;
	
	struct MYVERTEX
	{
		enum { FVF = D3DFVF_XYZ | D3DFVF_TEX1 };
		float px, py, pz;
		float tu, tv;
	};

	// 빌보드 정점
	MYVERTEX vtx[4] =
	{
		{ -1, 0, 0, 0, 1 },
		{ -1, 4, 0, 0, 0 },
		{ 1, 0, 0, 1, 1 },
		{ 1, 4, 0, 1, 0 }
	};

	D3DXMATRIXA16	matBillboard;
	D3DXMatrixIdentity( &matBillboard );

	// 0번 텍스처에 빌보드 텍스처를 올린다
	g_D3dDevice->SetTexture( 0, g_MeshTextures[id] );
	g_D3dDevice->SetFVF( MYVERTEX::FVF );

	g_D3dDevice->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, vtx, sizeof( MYVERTEX ) );
}

YAMANGDXDLL_API HRESULT MeshTextureCreateBySize( INT size )
{
	if ( size <= 0 || g_MeshTextureSize != 0)
	{
		return E_FAIL;
	}

	g_MeshTextures = new LPDIRECT3DTEXTURE9[size];
	g_MeshTextureSize = size;

	for ( int i = 0; i < g_MeshTextureSize; ++i )
	{
		g_MeshTextures[i] = nullptr;
	}

	return S_OK;
}

YAMANGDXDLL_API HRESULT MeshTextureImport( LPCTSTR meshTexture, int id )
{
	if ( FAILED( D3DXCreateTextureFromFile( g_D3dDevice, meshTexture, &g_MeshTextures[id] ) ) )
	{
		MessageBox( NULL, L"Mesh Texture Load Failed", meshTexture, MB_OK );
		return E_FAIL;
	}

	return S_OK;
}