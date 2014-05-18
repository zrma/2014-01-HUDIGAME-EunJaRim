#include "stdafx.h"
#include "yaMangDxDll.h"
#include "GlobalVar.h"
#include "InnerResource.h"


//////////////////////////////////////////////////////////////////////////
//render를 pre - main - post renderring으로 변경
//pre에서는 matrix, view, light에 대한 setting 진행
//main에서는 mesh object에 대한 직접 rendering 진행
//post에서는 pre에서 설정한 setting 초기화
//////////////////////////////////////////////////////////////////////////
YAMANGDXDLL_API bool PreRendering()
{
	if ( NULL == g_D3dDevice )
	{
		return false;
	}

	g_D3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB( 10, 10, 10 ), 1.0f, 0 );

	bool flag = false;

	// 렌더 방어코드
	// pre rendering 단계에서 진행되지 않으면 향후 render 모두 실패
	if ( SUCCEEDED( g_D3dDevice->BeginScene() ) )
	{
		SetupTranslateMatrices();

		// lightsetting
		// 일단 1로 진행, 향후 라이트 개수 등 확정되면 인자 받아 설정
		int lightNum = 1;
		Lighting( lightNum );
		// Log( "라이팅 세팅!\n" );

		g_D3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
		g_D3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
		g_D3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
		g_D3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_DISABLE );

		// Log( "Render Begin \n" );
		// Log( "pre render 완료!\n" );

		flag = true;
	}

	return flag;
}


YAMANGDXDLL_API void Rendering( MESHOBJECT* inputVal )
{
	g_D3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
	g_D3dDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );
	// Log( "Now Render : %p \n", inputVal );
	for ( DWORD i = 0; i < inputVal->NumMaterials; ++i )
	{
		g_D3dDevice->SetMaterial( &inputVal->MeshMarterials[i] );
		g_D3dDevice->SetTexture( 0, inputVal->MeshTexture[i] );

		( inputVal->importedMesh )->DrawSubset( i );
	}
}


YAMANGDXDLL_API void PostRendering()
{
	D3DXMATRIXA16 identityMatrix;
	D3DXMatrixIdentity( &identityMatrix );
	SetWorldAreaMatrix( &identityMatrix );

	g_D3dDevice->EndScene();

	//Log( "Render End \n" );
	g_D3dDevice->Present( NULL, NULL, NULL, NULL );
}

//////////////////////////////////////////////////////////////////////////
//heightMap을 위한 렌더 함수
//render를 pre - main - post renderring 구조에서 main을 담당
//////////////////////////////////////////////////////////////////////////
YAMANGDXDLL_API void RenderHeightMap()
{
	//현재 조명이 적용되지 않았음
	g_D3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
	g_D3dDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );

	IDirect3DVertexBuffer9* RenderVertexBuffer = nullptr;
	g_Mesh->GetVertexBuffer( &RenderVertexBuffer );

	IDirect3DIndexBuffer9* RenderIndexBuffer = nullptr;
	g_Mesh->GetIndexBuffer( &RenderIndexBuffer );

	g_D3dDevice->SetStreamSource( 0, RenderVertexBuffer, 0, sizeof( CUSTOMVERTEX ) );
	g_D3dDevice->SetFVF( D3DFVF_CUSTOMVERTEX );

	g_D3dDevice->SetTexture( 0, g_MapTexture );
	g_D3dDevice->SetTexture( 1, g_MapTextureArray[0] );

	g_D3dDevice->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, 0 );
	g_D3dDevice->SetTextureStageState( 1, D3DTSS_TEXCOORDINDEX, 1 );

	g_D3dDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
	g_D3dDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
	g_D3dDevice->SetSamplerState( 1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
	g_D3dDevice->SetSamplerState( 1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );

	g_D3dDevice->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_MODULATE );
	g_D3dDevice->SetTextureStageState( 1, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	g_D3dDevice->SetTextureStageState( 1, D3DTSS_COLORARG2, D3DTA_CURRENT );
	g_D3dDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP, D3DTOP_DISABLE );

	g_D3dDevice->SetIndices( RenderIndexBuffer );
	g_D3dDevice->DrawIndexedPrimitive( D3DPT_TRIANGLESTRIP, 0, 0, g_XHeight * g_ZHeight, 0, ( g_XHeight - 1 ) * ( g_ZHeight - 1 ) * 2 + ( g_ZHeight - 2 ) * 3 );
}

//////////////////////////////////////////////////////////////////////////
//Cursor를 위한 렌더 함수
//render를 pre - main - post renderring 구조에서 main을 담당
//////////////////////////////////////////////////////////////////////////
YAMANGDXDLL_API HRESULT RenderCursor()
{
	if ( g_CursorSprite && g_CursorTex && g_CursorTex[g_CursorType] )
	{
		D3DXMATRIXA16 ratioMat;

		float ratio = ( 720.0f / g_Width ) * g_Ratio;
		D3DXMatrixIdentity( &ratioMat );
		D3DXMatrixScaling( &ratioMat, 1280 / g_Width, ratio, 1 );
		g_CursorSprite->SetTransform( &ratioMat );
		g_CursorSprite->Begin( D3DXSPRITE_ALPHABLEND );
		g_CursorSprite->Draw( g_CursorTex[g_CursorType], NULL, NULL, &g_CursorPos, 0xFFFFFFFF );
		g_CursorSprite->End();
		g_D3dDevice->EndScene();
		return S_OK;
	}
	return E_FAIL;
}


//////////////////////////////////////////////////////////////////////////
//SkyBox를 위한 렌더 함수
//render를 pre - main - post renderring 구조에서 main을 담당
//////////////////////////////////////////////////////////////////////////
YAMANGDXDLL_API void RenderSkyBox()
{
	if ( g_SkyBoxMesh )
	{
		g_D3dDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );

		g_D3dDevice->SetSamplerState( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP );
		g_D3dDevice->SetSamplerState( 0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP );

		for ( int i = 0; i < 6; i++ )
		{
			g_D3dDevice->SetTexture( 0, g_SkyBoxTextures[i] );
			g_SkyBoxMesh->DrawSubset( i );
		}

		g_D3dDevice->SetSamplerState( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP );
		g_D3dDevice->SetSamplerState( 0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP );
	}
}


//////////////////////////////////////////////////////////////////////////
//캐릭터 툴을 위한 렌더 함수
//render를 pre - main - post renderring 구조에서 main을 담당
//////////////////////////////////////////////////////////////////////////
YAMANGDXDLL_API void RenderingTool( MESHOBJECT* inputVal )
{
	if ( NULL == g_D3dDevice )
	{
		return;
	}

	if ( g_Mesh == nullptr )
	{
		InitGroundMesh( 100, 100 );
	}
	g_D3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB( 30, 10, 10 ), 1.0f, 0 );

	// 렌더 방어코드
	if ( SUCCEEDED( g_D3dDevice->BeginScene() ) )
	{
		SetupTranslateMatricesTool();
		// ViewSetting();

		// lightsetting
		// 일단 1로 진행, 향후 라이트 개수 등 확정되면 인자 받아 설정
		int lightNum = 1;
		Lighting( lightNum );
		// Log( "라이팅 세팅!\n" );

		g_D3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
		g_D3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
		g_D3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
		g_D3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_DISABLE );

		// Log( "Render Begin \n" );
		// Log( "pre render 완료!\n" );
	}

	// 카메라 셋팅
	D3DXMATRIXA16 viewMatrix;
	D3DXMatrixLookAtLH( &viewMatrix, &g_EyePoint, &g_LookAtPoint, &g_UpVector );
	SetCameraMatrix( &viewMatrix );

	// 보여주기 위한 땅을 만듬
	//InitGroundMesh(100, 100);
	CreateRawGround( 100, 100, 10 );
	RenderHeightMap();

	g_D3dDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );

	// Log( "Now Render : %p \n", inputVal );
	for ( DWORD i = 0; i < inputVal->NumMaterials; ++i )
	{
		g_D3dDevice->SetMaterial( &inputVal->MeshMarterials[i] );
		g_D3dDevice->SetTexture( 0, inputVal->MeshTexture[i] );

		( inputVal->importedMesh )->DrawSubset( i );
	}
	g_D3dDevice->EndScene();

	// Log( "Render End \n" );
	g_D3dDevice->Present( NULL, NULL, NULL, NULL );
}