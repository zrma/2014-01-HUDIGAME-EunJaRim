// yaMangDxDll.cpp : DLL 응용 프로그램을 위해 내보낸 함수를 정의합니다.
//

#include "stdafx.h"
#include "yaMangDxDll.h"
#include "GlobalVar.h"


//////////////////////////////////////////////////////////////////////////
//Text Render
//////////////////////////////////////////////////////////////////////////
YAMANGDXDLL_API void RenderText( LPCWSTR text, float left, float top, int RGB_R, int RGB_G, int RGB_B, float right, float bottom )
{
	g_Sprite->Begin( D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE );
	// 스프라이트 그리기 시작
	// 2D일 경우 D3DXSPRITE_SORT_TEXTURE, 3D일 경우 D3DXSPRITE_OBJECTSPACE
	RECT rt = {
		static_cast<LONG>( left ),
		static_cast<LONG>( top ),
		static_cast<LONG>( right ),
		static_cast<LONG>( bottom )
	}; // 그릴 위치
	
	// 비율 고정
	D3DXMATRIXA16 ratioMat;
	D3DXMatrixIdentity( &ratioMat );
	D3DXMatrixScaling( &ratioMat, g_StartWidth / g_NowWidth, g_StartHeight / g_NowHeight, 1 );
	g_Sprite->SetTransform( &ratioMat );

	g_Font->DrawText( g_Sprite, text
					  , -1										// 총 문자열 수(그냥 -1 해도 됨.) 
					  , &rt									// 그릴 위치 
					  , DT_NOCLIP								// 옵션 플래그
					  , D3DCOLOR_XRGB( RGB_R, RGB_G, RGB_B )	// 그릴 색 , default로 흰색
					  );
	g_Sprite->End();											// 스프라이트 그리기 끝 
}


//매트릭스 출력용
//Log("%f, %f, %f, %f \n", matrix->_11, matrix->_12, matrix->_13, matrix->_14);
//Log("%f, %f, %f, %f \n", matrix->_21, matrix->_22, matrix->_23, matrix->_24);
//Log("%f, %f, %f, %f \n", matrix->_31, matrix->_32, matrix->_33, matrix->_34);
//Log("%f, %f, %f, %f \n", matrix->_41, matrix->_42, matrix->_43, matrix->_44);
//Log("==============================");


YAMANGDXDLL_API void SetCameraMatrix( D3DXMATRIXA16* matrix )
{
	g_D3dDevice->SetTransform( D3DTS_VIEW, matrix );
}

YAMANGDXDLL_API void SetWorldAreaMatrix( D3DXMATRIXA16* matrix )
{
	g_D3dDevice->SetTransform( D3DTS_WORLD, matrix );
}


YAMANGDXDLL_API void SetAspectRatio( long width, long height )
{
	D3DXMATRIXA16 matProj;
	float aspectRatio = static_cast<float>( width ) / static_cast<float>( height );
	
	D3DXMatrixPerspectiveFovLH( &matProj, D3DX_PI / 5, aspectRatio, 1.0f, 2000.0f );
	g_D3dDevice->SetTransform( D3DTS_PROJECTION, &matProj );

	g_NowWidth = static_cast<float>( width );
	g_NowHeight = static_cast<float>( height );
}
