#include "stdafx.h"
#include "yaMangDxDll.h"
#include "GlobalVar.h"

//////////////////////////////////////////////////////////////////////////
// UI
//////////////////////////////////////////////////////////////////////////
YAMANGDXDLL_API HRESULT InitUISprite( int textureSize )
{
	if ( FAILED( D3DXCreateSprite( g_D3dDevice, &g_UISprite ) ) ) //first parameter is our device, second is a empty sprite variable
	{
		return E_FAIL;
	}
	if ( g_UITexures )
	{
		for ( int i = 0; i < g_UIMaxSize; ++i )
		{
			if ( g_UITexures[i] )
			{
				g_UITexures[i]->Release();
			}
		}
		delete[] g_UITexures;
		g_UITexures = nullptr;
	}

	g_UITexures = new LPDIRECT3DTEXTURE9[textureSize];
	g_UIMaxSize = textureSize;

	for ( int i = 0; i < g_UIMaxSize; ++i )
	{
		g_UITexures[i] = nullptr;
	}

	Log( " UI 스프라이트 %d개 생성 ", g_UIMaxSize );

	return S_OK;
}

YAMANGDXDLL_API HRESULT CreateUIImage( LPCWSTR ImagePath /*= L"UI_default.png"*/, INT typeNum /*= 0*/ )
{
	if ( g_D3dDevice )
	{
		if ( g_UITexures[typeNum] )
		{
			g_UITexures[typeNum]->Release();
			g_UITexures[typeNum] = nullptr;
		}
		if ( FAILED( D3DXCreateTextureFromFile( g_D3dDevice, ImagePath, &g_UITexures[typeNum] ) ) )
		{
			return E_FAIL;
		}

		return S_OK;
	}

	return E_FAIL;
}

YAMANGDXDLL_API HRESULT RenderUI( INT typeNum, D3DXVECTOR3 UIPos )
{
	if ( g_UISprite )
	{
		D3DXMATRIXA16 ratioMat;

		float ratio = ( 720.0f / g_Width ) * g_Ratio;
		D3DXMatrixIdentity( &ratioMat );
		D3DXMatrixScaling( &ratioMat, 1280 / g_Width, ratio, 1 );
		g_UISprite->SetTransform( &ratioMat );

		g_UISprite->Begin( D3DXSPRITE_ALPHABLEND );
		g_UISprite->Draw( g_UITexures[typeNum], NULL, NULL, &UIPos, 0xFFFFFFFF );
		g_UISprite->End();

		g_D3dDevice->EndScene();
		return S_OK;
	}
	return E_FAIL;
}