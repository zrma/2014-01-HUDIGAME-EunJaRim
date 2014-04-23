// yaMangDxDll.cpp : DLL 응용 프로그램을 위해 내보낸 함수를 정의합니다.
//

#include "stdafx.h"
#include "yaMangDxDll.h"
#include "Logger.h"
#include <stdio.h>
#include "GlobalVar.h"
#include "InnerResource.h"


//////////////////////////////////////////////////////////////////////////
//input args: 윈도우 핸들
//향후 공용으로 사용할 D3D, D3DDevice에 대해 초기화 진행
//프로그램에서 1회만 실행 후 Process 종료까지 사용
//참고 URL : http://www.delmadang.com/community/bbs_view.asp?bbsNo=17&indx=426040 
//////////////////////////////////////////////////////////////////////////
YAMANGDXDLL_API HRESULT InitD3D( HWND hWnd, long width, long height )
{
	if ( nullptr == ( g_D3D = Direct3DCreate9( D3D_SDK_VERSION ) ) )
	{
		// 오류
		MessageBox( NULL, L"Could not Create D3D", L"YaMang.exe", MB_OK );
		return E_FAIL;
	}

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory( &d3dpp, sizeof( d3dpp ) );
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	if ( FAILED( g_D3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, 
		hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &g_D3dDevice ) ) )
	{
		MessageBox( NULL, L"Could not CreateDevice", L"YaMang.exe", MB_OK );
		return E_FAIL;
	}

	g_D3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
	g_D3dDevice->SetRenderState( D3DRS_ZENABLE, TRUE );

	SetAspectRatio( width, height );

	//텍스트 출력 자원 초기화
	D3DXCreateFont(
		g_D3dDevice						// Device 객체 
		, 30					// 폰트 높이 
		, 0					// 폰트 길이 
		, FW_NORMAL						// 폰트 굵기 (기본 FW_BOLD) 
		, 1								// 밉레벨 
		, false							// 폰트 기울기 켜기/끄기 설정 
		, DEFAULT_CHARSET				// 문자셋 
		, OUT_DEFAULT_PRECIS			// 출력 정확도 설정 
		, DEFAULT_QUALITY				// 퀄리티 설정 
		, DEFAULT_PITCH | FF_DONTCARE	// 피치 설정 
		, L"맑은 고딕"						// 글꼴 설정 
		, &g_Font						// 초기화할 Font 
		);								// 폰트 초기화

	D3DXCreateSprite( g_D3dDevice, &g_Sprite ); // 스프라이트 초기화 
#ifdef _PRINT_CONSOLE
	Logger::GetInstance();
#endif

	return S_OK;
}

YAMANGDXDLL_API HRESULT InitGeometry( HWND hWnd, LPCTSTR fileName, MESHOBJECT* inputVal )
{
	LPD3DXBUFFER D3dxMtrialBuffer;

	//x file import
	if ( FAILED( D3DXLoadMeshFromX( fileName, D3DXMESH_SYSTEMMEM, g_D3dDevice, NULL, 
		&D3dxMtrialBuffer, NULL, &( inputVal->NumMaterials ), &inputVal->importedMesh ) ) )
	{
		MessageBox( NULL, L"Could not find x file", L"YaMang.exe", MB_OK );
		return E_FAIL;
	}

	//import한 mesh에 normal 값이 없는 경우 normal 계산
	if ( !( ( inputVal->importedMesh )->GetFVF()&D3DFVF_NORMAL ) )
	{
		ID3DXMesh* pTempMesh = 0;
		( inputVal->importedMesh )->CloneMeshFVF( D3DXMESH_MANAGED, ( inputVal->importedMesh )->GetFVF() | D3DFVF_NORMAL, g_D3dDevice, &pTempMesh );

		D3DXComputeNormals( pTempMesh, 0 );

		( inputVal->importedMesh )->Release();
		( inputVal->importedMesh ) = pTempMesh;
	}

	D3DXMATERIAL* d3dxMarteials = (D3DXMATERIAL*)D3dxMtrialBuffer->GetBufferPointer();
	inputVal->MeshMarterials = new D3DMATERIAL9[inputVal->NumMaterials];
	if ( nullptr == inputVal->MeshMarterials )
	{
		return E_OUTOFMEMORY;
	}

	inputVal->MeshTexture = new LPDIRECT3DTEXTURE9[inputVal->NumMaterials];
	if ( nullptr == inputVal->MeshTexture )
	{
		return E_OUTOFMEMORY;
	}

	for ( DWORD i = 0; i < inputVal->NumMaterials; ++i )
	{
		inputVal->MeshMarterials[i] = d3dxMarteials[i].MatD3D;

		inputVal->MeshMarterials[i].Ambient = inputVal->MeshMarterials[i].Diffuse;

		inputVal->MeshTexture[i] = NULL;
		if ( ( NULL != d3dxMarteials[i].pTextureFilename ) && lstrlenA( d3dxMarteials[i].pTextureFilename )>0 )
		{
			if ( FAILED( D3DXCreateTextureFromFileA( g_D3dDevice, d3dxMarteials[i].pTextureFilename, &( inputVal->MeshTexture[i] ) ) ) )
			{
				const CHAR* strPrefix = "..\\";
				CHAR strTexture[MAX_PATH];
				strcpy_s( strTexture, MAX_PATH, strPrefix );
				strcat_s( strTexture, MAX_PATH, d3dxMarteials[i].pTextureFilename );

				if ( FAILED( D3DXCreateTextureFromFileA( g_D3dDevice, strTexture, &( inputVal->MeshTexture[i] ) ) ) )
				{
					MessageBox( NULL, L"Could not find texture map", L"YaMang.exe", MB_OK );
				}
			}
		}
	}


	D3dxMtrialBuffer->Release();

	return S_OK;
}



YAMANGDXDLL_API void SetAspectRatio( long width, long height )
{
	D3DXMATRIXA16 matProj;
	float aspectRatio = static_cast<float>( width ) / static_cast<float>( height );

	D3DXMatrixPerspectiveFovLH( &matProj, D3DX_PI / 5, aspectRatio, 1.0f, 2000.0f );
	g_D3dDevice->SetTransform( D3DTS_PROJECTION, &matProj );
}


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

	//렌더 방어코드
	//pre rendering 단계에서 진행되지 않으면 향후 render 모두 실패
	if ( SUCCEEDED( g_D3dDevice->BeginScene() ) )
	{
		SetupTranslateMatrices();
		//ViewSetting();

		//lightsetting
		//일단 1로 진행, 향후 라이트 개수 등 확정되면 인자 받아 설정
		int lightNum = 1;
		Lighting( lightNum );
		// Log( "라이팅 세팅!\n" );

		g_D3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
		g_D3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
		g_D3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
		g_D3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_DISABLE );

		//Log( "Render Begin \n" );
		//Log( "pre render 완료!\n" );

		flag = true;
	}

	return flag;
}


YAMANGDXDLL_API void Rendering( MESHOBJECT* inputVal )
{
	//Log( "Now Render : %p \n", inputVal );
	for ( DWORD i = 0; i < inputVal->NumMaterials; ++i )
	{
		g_D3dDevice->SetMaterial( &inputVal->MeshMarterials[i] );
		g_D3dDevice->SetTexture( 0, inputVal->MeshTexture[i] );

		( inputVal->importedMesh )->DrawSubset( i );
	}
}


YAMANGDXDLL_API void PostRendering()
{
	g_D3dDevice->EndScene();

	//Log( "Render End \n" );
	g_D3dDevice->Present( NULL, NULL, NULL, NULL );
}


YAMANGDXDLL_API void RenderingTool( MESHOBJECT* inputVal )
{
	if ( NULL == g_D3dDevice )
	{
		return;
	}

	g_D3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB( 30, 10, 10 ), 1.0f, 0 );

	//렌더 방어코드
	if ( SUCCEEDED( g_D3dDevice->BeginScene() ) )
	{
		SetupTranslateMatricesTool();
		//ViewSetting();

		//lightsetting
		//일단 1로 진행, 향후 라이트 개수 등 확정되면 인자 받아 설정
		int lightNum = 1;
		Lighting( lightNum );
		//Log( "라이팅 세팅!\n" );

		g_D3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
		g_D3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
		g_D3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
		g_D3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_DISABLE );

		//Log( "Render Begin \n" );
		//Log( "pre render 완료!\n" );
	}

	//카메라 셋팅
	D3DXMATRIXA16 viewMatrix;
	D3DXMatrixLookAtLH( &viewMatrix, &g_EyePoint, &g_LookAtPoint, &g_UpVector );
	SetMatrix( &viewMatrix , true);

	//Log( "Now Render : %p \n", inputVal );
	for ( DWORD i = 0; i < inputVal->NumMaterials; ++i )
	{
		g_D3dDevice->SetMaterial( &inputVal->MeshMarterials[i] );
		g_D3dDevice->SetTexture( 0, inputVal->MeshTexture[i] );

		( inputVal->importedMesh )->DrawSubset( i );
	}
	g_D3dDevice->EndScene();

	//Log( "Render End \n" );
	g_D3dDevice->Present( NULL, NULL, NULL, NULL );
}

YAMANGDXDLL_API void SetCameraTool( float x, float y, float z )
{

}

YAMANGDXDLL_API void MeshObjectCleanUp( MESHOBJECT* inputVal )
{
	if ( NULL != inputVal->MeshMarterials )
	{
		delete[] inputVal->MeshMarterials;
	}
	if ( inputVal->MeshTexture )
	{
		for ( DWORD i = 0; i < inputVal->NumMaterials; ++i )
		{
			if ( inputVal->MeshTexture[i] )
			{
				inputVal->MeshTexture[i]->Release();
			}
		}
		delete[] inputVal->MeshTexture;
	}
	if ( NULL != inputVal->importedMesh )
	{
		(inputVal->importedMesh)->Release();
	}
}


YAMANGDXDLL_API void D3DCleanUp()
{
	if ( NULL != g_D3dDevice )
	{
		g_D3dDevice->Release();
	}

	if ( NULL != g_D3D )
	{
		g_D3D->Release();
	}

	// 텍스트 소멸자 합침
	if ( g_Font != NULL )
	{
		g_Font->Release();
	}

	if ( g_Sprite != NULL )
	{
		g_Sprite->Release();
	}

#ifdef _PRINT_CONSOLE
	Logger::Release();
#endif
}


YAMANGDXDLL_API void SetMatrix( D3DXMATRIXA16* matrix, bool cameraSet /*= false */ )
{
	//Log("%f, %f, %f, %f \n", matrix->_11, matrix->_12, matrix->_13, matrix->_14);
	//Log("%f, %f, %f, %f \n", matrix->_21, matrix->_22, matrix->_23, matrix->_24);
	//Log("%f, %f, %f, %f \n", matrix->_31, matrix->_32, matrix->_33, matrix->_34);
	//Log("%f, %f, %f, %f \n", matrix->_41, matrix->_42, matrix->_43, matrix->_44);
	//Log("==============================");

	if ( cameraSet == true )
	{
		g_D3dDevice->SetTransform( D3DTS_VIEW, matrix );
	}
	else
	{
		g_D3dDevice->SetTransform( D3DTS_WORLD, matrix );
	}
}


//////////////////////////////////////////////////////////////////////////
//height map 세계에 오신 것을 환영합니다.
// :)
//////////////////////////////////////////////////////////////////////////

YAMANGDXDLL_API HRESULT HeightMapTextureImport ( HWND hWnd, LPCTSTR heightMap, LPCTSTR mapTexture )
{
	if ( FAILED( D3DXCreateTextureFromFileEx( g_D3dDevice, heightMap, D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, D3DFMT_X8B8G8R8, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &g_TexHeight ) ) )
	{
		MessageBox( NULL, L"Could not find heightMap file", L"YaMang.exe", MB_OK );
		return E_FAIL;
	}

	if ( FAILED( D3DXCreateTextureFromFile( g_D3dDevice, mapTexture, &g_TexDiffuse ) ) )
	{
		MessageBox( NULL, L"Could not find heightMapTexture file", L"YaMang.exe", MB_OK );
		return E_FAIL;
	}

	if ( FAILED( InitVertexBuffer( hWnd ) ) )
	{
		MessageBox( NULL, L"Fail in InitVertexBuffer", L"YaMang.exe", MB_OK );
		return E_FAIL;
	}

	if ( FAILED( InitIdxBuffer( hWnd ) ) )
	{
		MessageBox( NULL, L"Fail in InitIdxBuffer", L"YaMang.exe", MB_OK );
		return E_FAIL;
	}
	
	return S_OK;
}


YAMANGDXDLL_API void HeightMapCleanup()
{
	if ( g_TexHeight != NULL )
	{
		g_TexHeight->Release();
	}

	if ( g_TexDiffuse != NULL )
	{
		g_TexDiffuse->Release();
	}

	if ( g_IdxBuffer != NULL )
	{
		g_IdxBuffer->Release();
	}

	if ( g_VertexBuffer != NULL )
	{
		g_VertexBuffer->Release();
	}

}

YAMANGDXDLL_API void HeightMapRender()
{
	/*Log("start");
	//카메라 셋팅
	D3DXMATRIXA16 world;
	D3DXMatrixIdentity(&world);
	g_D3dDevice->SetTransform(D3DTS_WORLD, &world);

	D3DXVECTOR3 vEyePt(0.f, 300.f, -1000.f);
	D3DXVECTOR3 vLookatPt(0.f, 299.7f, -999.0f);
	D3DXVECTOR3 vUpVec(0.f, 1.f, 0.f);
	D3DXMATRIXA16 matView;
	D3DXMatrixLookAtLH(&matView, &vEyePt, &vLookatPt, &vUpVec);
	g_D3dDevice->SetTransform(D3DTS_VIEW, &matView);
	Log("end\n");

	D3DXMATRIXA16 worldMatrix;
	g_D3dDevice->GetTransform(D3DTS_WORLD, &worldMatrix);

	Log("%f, %f, %f, %f \n", worldMatrix._11, worldMatrix._12, worldMatrix._13, worldMatrix._14);
	Log("%f, %f, %f, %f \n", worldMatrix._21, worldMatrix._22, worldMatrix._23, worldMatrix._24);
	Log("%f, %f, %f, %f \n", worldMatrix._31, worldMatrix._32, worldMatrix._33, worldMatrix._34);
	Log("%f, %f, %f, %f \n", worldMatrix._41, worldMatrix._42, worldMatrix._43, worldMatrix._44);
	Log("==============================");

	SetAspectRatio(729, 588);*/

	g_D3dDevice->SetStreamSource( 0, g_VertexBuffer, 0, sizeof( CUSTOMVERTEX ) );
	g_D3dDevice->SetFVF( D3DFVF_CUSTOMVERTEX );

	g_D3dDevice->SetTexture( 0, g_TexDiffuse );

	g_D3dDevice->SetIndices( g_IdxBuffer );
	Log("go\n");
	g_D3dDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, g_XHeight*g_ZHeight, 0, ( g_XHeight - 1 )*( g_ZHeight - 1 ) * 2 );
	Log("stop\n");
}

//////////////////////////////////////////////////////////////////////////
//Text Render
//////////////////////////////////////////////////////////////////////////

YAMANGDXDLL_API void RenderText( LPCWSTR text, float left, float top, int RGB_R, int RGB_G, int RGB_B, float right, float bottom)
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
	g_Font->DrawText( g_Sprite, text
					, -1										// 총 문자열 수(그냥 -1 해도 됨.) 
					, &rt									// 그릴 위치 
					, DT_NOCLIP								// 옵션 플래그
					, D3DCOLOR_XRGB( RGB_R, RGB_G, RGB_B )	// 그릴 색 , default로 흰색
					);
	g_Sprite->End();											// 스프라이트 그리기 끝 
}

//////////////////////////////////////////////////////////////////////////
// Picking Ray를 위한 부분
//////////////////////////////////////////////////////////////////////////

YAMANGDXDLL_API void CalcPickingRay( int mouseX, int mouseY, D3DXVECTOR3* rayOrigin, D3DXVECTOR3* rayDirection )
{
	D3DVIEWPORT9 veiwPort;
	D3DXMATRIXA16 projectionMatrix;

	float rayX = 0.f;
	float rayY = 0.f;

	g_D3dDevice->GetViewport( &veiwPort );
	g_D3dDevice->GetTransform( D3DTS_PROJECTION, &projectionMatrix );

	rayX = ( ( ( mouseX *2.f ) / (float) veiwPort.Width ) - 1.f ) / projectionMatrix._11;
	rayY = ( ( ( mouseY *-2.f ) / (float) veiwPort.Height ) + 1.f ) / projectionMatrix._22;

	//viewport트랜스, 프로젝션 트랜스 역행
	*rayOrigin = D3DXVECTOR3( 0.f, 0.f, 0.f );
	*rayDirection = D3DXVECTOR3( rayX, rayY, 1.f );
	Log( "뷰포트, 프로젝션 역행" );

	//뷰잉 트랜스 역행
	D3DXMATRIXA16 viewingMatrix;
	g_D3dDevice->GetTransform( D3DTS_VIEW, &viewingMatrix );
	D3DXMatrixInverse( &viewingMatrix, 0, &viewingMatrix );

	D3DXVec3TransformCoord( rayOrigin, rayOrigin, &viewingMatrix );
	D3DXVec3TransformCoord( rayDirection, rayDirection, &viewingMatrix );
	Log( "뷰잉 좌표 역행" );

	//월드 좌표로 역행
	D3DXMATRIXA16 worldMatrix;
	g_D3dDevice->GetTransform( D3DTS_WORLD, &worldMatrix );
	D3DXMatrixInverse( &worldMatrix, 0, &worldMatrix );

	D3DXVec3TransformCoord( rayOrigin, rayOrigin, &worldMatrix );
	D3DXVec3TransformCoord( rayDirection, rayDirection, &worldMatrix );
	Log( "월드 좌표 역행" );
	Log( "origin: %f,%f,%f\n direction: %f, %f, %f\n", rayOrigin->x, rayOrigin->y, rayOrigin->z, rayDirection->x, rayDirection->y, rayDirection->z );

}

//////////////////////////////////////////////////////////////////////////
/// D3D cursor Set을 위한 부분
//////////////////////////////////////////////////////////////////////////
YAMANGDXDLL_API void SetD3DCursor( LPCWSTR textureName )
{
	//테스트 중
	//d3d 자체 커서 만드는 방법 1
	// 	D3DXLoadSurfaceFromFile( g_surfcursor, NULL, NULL, textureName, NULL, D3DX_FILTER_NONE, 0, NULL );
	// 	g_D3dDevice->SetCursorProperties( 0, 0, g_surfcursor );
	// 	g_D3dDevice->SetCursorPosition( 220, 220, D3DCURSOR_IMMEDIATE_UPDATE );
	// 	g_D3dDevice->ShowCursor( TRUE );

	//d3d 자체 커서 만드는 방법 2
	// 	g_D3dDevice->CreateTexture( 32, 32, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &g_cursortex, nullptr );
	// 	g_D3dDevice->CreateOffscreenPlainSurface( 32, 32, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &g_surfcursor, nullptr );
	// 	
	// 	D3DXCreateTextureFromFile( g_D3dDevice, L"cursor1.bmp", &g_cursortex );
	// 
	// 	g_cursortex->GetSurfaceLevel( 0, &g_surfcursor );
	// 	g_D3dDevice->SetCursorProperties( 0, 0, g_surfcursor );
	// 
	// 	g_D3dDevice->ShowCursor( TRUE );
	
}

YAMANGDXDLL_API HRESULT InitCursor( LPCWSTR cursorImagePath, float cursorPosX, float cursorPosY )
{
	if ( FAILED( D3DXCreateTextureFromFile( g_D3dDevice, cursorImagePath, &g_cursorTex ) ) )
	{
		return E_FAIL;
	}
	if ( FAILED( D3DXCreateSprite( g_D3dDevice, &g_cursorSprite ) ) )//first parameter is our device, second is a empty sprite variable
	{
		return E_FAIL;
	}

	SetCursorPosition();// 0,0,0으로 초기화

	return S_OK;
}

YAMANGDXDLL_API HRESULT CursorRender( )
{
	if ( g_cursorSprite ) 
	{
		g_cursorSprite->Begin( D3DXSPRITE_ALPHABLEND ); 
		g_cursorSprite->Draw( g_cursorTex, NULL, NULL, &g_cursorPos, 0xFFFFFFFF ); 
		g_cursorSprite->End(); 
		g_D3dDevice->EndScene();
		return S_OK;
	}
	return E_FAIL;
}

YAMANGDXDLL_API void CursorCleanUp( )
{
	if ( g_cursorSprite )
	{
		g_cursorSprite->Release();
	}

	if ( g_cursorTex )
	{
		g_cursorTex->Release();
	}
}

YAMANGDXDLL_API void SetCursorPosition( float PosX, float PosY )
{
	g_cursorPos.x = PosX;
	g_cursorPos.y = PosY;
	g_cursorPos.z = 0.0f;
}

// 내보낸 변수의 예제입니다.
// YAMANGDXDLL_API int nyaMangDxDll=0;
