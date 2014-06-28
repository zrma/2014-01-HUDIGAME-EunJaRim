#include "stdafx.h"
#include "yaMangDxDll.h"
#include "GlobalVar.h"
#include <timeapi.h>

enum RenderingOption
{
	SETTING_NONE,

	LIGHT_SETTING_ON,
	LIGHT_SETTING_OFF,
};

YAMANGDXDLL_API void SetEffect( bool isEffect )
{
	g_IsEffectReady = isEffect;
}

YAMANGDXDLL_API void Rendering( MESHOBJECT* inputVal )
{
	if ( g_IsEffectReady && g_Effects[0] )
	{
		D3DVERTEXELEMENT9	ele[MAX_FVF_DECL_SIZE];

		// FVF를 사용해서 정점선언값을 자동으로 채워넣는다
		D3DXDeclaratorFromFVF( D3DFVF_CUSTOMVERTEX, ele );
		LPDIRECT3DVERTEXDECLARATION9	decl;

		// 정점선언값으로 decl을 생성한다.
		g_D3dDevice->CreateVertexDeclaration( ele, &decl );
		g_D3dDevice->SetVertexDeclaration( decl );

		UINT nPass;
		float thisTime = D3DX_PI * (timeGetTime() % 1400) / 700;

		g_Effects[0]->SetFloat( "time", thisTime );

		// fx출력에 사용할 테크닉 선정
		g_Effects[0]->SetTechnique( "MyShader" );
				
		D3DXMATRIXA16 worldMatrix;
		g_D3dDevice->GetTransform( D3DTS_WORLD, &worldMatrix );
		D3DXMATRIXA16 viewingMatrix;
		g_D3dDevice->GetTransform( D3DTS_VIEW, &viewingMatrix );
		D3DXMATRIXA16 projectionMatrix;
		g_D3dDevice->GetTransform( D3DTS_PROJECTION, &projectionMatrix );

		D3DXMATRIXA16 thisMatrix = worldMatrix * viewingMatrix * projectionMatrix;

		g_Effects[0]->SetMatrix( "matWVP", &thisMatrix );

		g_Effects[0]->SetTexture( "tex0", inputVal->MeshTexture[0] );

		// fx를 사용한 출력개시
		g_Effects[0]->Begin( &nPass, D3DXFX_DONOTSAVESHADERSTATE );
		
		// PASS 개수만큼 출력
		for ( UINT i = 0; i < nPass; ++i )
		{
			g_Effects[0]->BeginPass( i );
			
			for ( DWORD j = 0; j < inputVal->NumMaterials; ++j )
			{
				( inputVal->importedMesh )->DrawSubset( j );
			}

			g_Effects[0]->EndPass();
		}

		/// fx를 사용한 출력종료
		g_Effects[0]->End();
	}
	else
	{
		g_D3dDevice->SetVertexShader( NULL );
		g_D3dDevice->SetPixelShader( NULL );

		g_D3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
		g_D3dDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );

		for ( DWORD i = 0; i < inputVal->NumMaterials; ++i )
		{
			g_D3dDevice->SetMaterial( &inputVal->MeshMarterials[i] );
			g_D3dDevice->SetTexture( 0, inputVal->MeshTexture[i] );

			( inputVal->importedMesh )->DrawSubset( i );
		}
	}

	g_IsEffectReady = false;
}

//////////////////////////////////////////////////////////////////////////
// 정리 완료
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
	//g_D3dDevice->SetTexture( 1, g_MapTextureArray[1] );

	//설정된 uv 좌표값에 따라 연산
	g_D3dDevice->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, 0 );

	g_D3dDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
	g_D3dDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
	
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
		// 비율 고정
		D3DXMATRIXA16 ratioMat;
		D3DXMatrixIdentity( &ratioMat );
		D3DXMatrixScaling( &ratioMat, g_StartWidth / g_NowWidth, g_StartHeight / g_NowHeight, 1 );
		g_CursorSprite->SetTransform( &ratioMat );

		g_CursorSprite->Begin( D3DXSPRITE_ALPHABLEND );
		g_CursorSprite->Draw( g_CursorTex[g_CursorType], NULL, NULL, &g_CursorPos, 0xFFFFFFFF );
		g_CursorSprite->End();
		g_D3dDevice->EndScene();
		return S_OK;
	}
	return E_FAIL;
}