// yaMangDxDll.cpp : DLL 응용 프로그램을 위해 내보낸 함수를 정의합니다.
//

#include "stdafx.h"
#include "yaMangDxDll.h"
#include "Logger.h"
#include <stdio.h>

//////////////////////////////////////////////////////////////////////////
//Global val 선언
//////////////////////////////////////////////////////////////////////////
//부모 Process 공용 자원
LPDIRECT3D9 g_D3D = nullptr;
LPDIRECT3DDEVICE9 g_D3dDevice = nullptr;


//////////////////////////////////////////////////////////////////////////
//input args: 윈도우 핸들
//향후 공용으로 사용할 D3D, D3DDevice에 대해 초기화 진행
//프로그램에서 1회만 실행 후 Process 종료까지 사용
//참고url: http://www.delmadang.com/community/bbs_view.asp?bbsNo=17&indx=426040 
//////////////////////////////////////////////////////////////////////////
YAMANGDXDLL_API HRESULT InitD3D( HWND hWnd )
{
	if ( nullptr == ( g_D3D = Direct3DCreate9( D3D_SDK_VERSION ) ) )
	{
		// 오류
		MessageBox( NULL, L"Could not Create D3D", L"Meshes.exe", MB_OK );
		return E_FAIL;
	}

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory( &d3dpp, sizeof( d3dpp ) );
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

	if ( FAILED( g_D3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &g_D3dDevice ) ) )
	{
		MessageBox( NULL, L"Could not CreateDevice", L"Meshes.exe", MB_OK );
		return E_FAIL;
	}

	g_D3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
	g_D3dDevice->SetRenderState( D3DRS_ZENABLE, TRUE );

#ifdef _PRINT_CONSOLE
	Logger::GetInstance();
#endif

	return S_OK;
}

YAMANGDXDLL_API HRESULT InitGeometry( HWND hWnd, LPCTSTR fileName, MESHOBJECT* inputVal )
{
	LPD3DXBUFFER D3dxMtrialBuffer;

	//x file import
	if ( FAILED( D3DXLoadMeshFromX( fileName, D3DXMESH_SYSTEMMEM, g_D3dDevice, NULL, &D3dxMtrialBuffer, NULL, &( inputVal->NumMaterials ), &inputVal->importedMesh ) ) )
	{
		MessageBox( NULL, L"Could not find x file", L"Mesh Load", MB_OK );
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
					MessageBox( NULL, L"Could not find texture map", L"Meshes.exe", MB_OK );
				}
			}
		}
	}


	D3dxMtrialBuffer->Release();

	return S_OK;
}

void SetupTranslateMatrices( float moveX, float moveY, float moveZ )
{
	D3DXMATRIXA16 matWorld;
	D3DXMatrixIdentity( &matWorld );
	g_D3dDevice->SetTransform( D3DTS_WORLD, &matWorld );

	//x, y, z축 입력 값에 대해 이동 처리
	D3DXMATRIXA16 thisMatrix;
	D3DXMatrixTranslation( &thisMatrix, moveX, moveY, moveZ );
	g_D3dDevice->MultiplyTransform( D3DTS_WORLD, &thisMatrix );

	//향후 추가 매트릭스 처리 필요 내용에 대해 추가 예정
}

//카메라 세팅은 향후에 진행하도록 함
//default로 일단 이렇게 둠
void ViewSetting()
{
	D3DXVECTOR3 vEyePt( 0.f, 3.f, -5.f );
	D3DXVECTOR3 vLookatPt( 0.f, 0.f, 0.f );
	D3DXVECTOR3 vUpVec( 0.f, 1.f, 0.f );
	D3DXMATRIXA16 matView;
	D3DXMatrixLookAtLH( &matView, &vEyePt, &vLookatPt, &vUpVec );
	SetMatrix( &matView, true );

	D3DXMATRIXA16 matProj;
	D3DXMatrixPerspectiveFovLH( &matProj, D3DX_PI / 2, 1.0f, 1.0f, 100.0f );
	g_D3dDevice->SetTransform( D3DTS_PROJECTION, &matProj );
}

//light 세팅에 대해서는 향후 추가
//light는 여러개를 미리 가지고 있게 할 것인가?
//아니면 사용자가 최소로 추가하고 해당 light를 공용 자원으로 할 것인가?
void Lighting( int lightNum )
{
	//재질 속성 부여
	D3DMATERIAL9 mtrl;
	ZeroMemory( &mtrl, sizeof( D3DMATERIAL9 ) );
	mtrl.Diffuse.r = mtrl.Ambient.r = 1.0f;
	mtrl.Diffuse.g = mtrl.Ambient.g = 1.0f;
	mtrl.Diffuse.b = mtrl.Ambient.b = 1.0f;
	mtrl.Diffuse.a = mtrl.Ambient.a = 1.0f;
	g_D3dDevice->SetMaterial( &mtrl );

	D3DXVECTOR3 vecDir0;
	D3DLIGHT9 light0;
	ZeroMemory( &light0, sizeof( D3DLIGHT9 ) );
	light0.Type = D3DLIGHT_DIRECTIONAL;
	light0.Diffuse.r = 1.f;
	light0.Diffuse.g = 1.f;
	light0.Diffuse.b = 1.f;
	vecDir0 = D3DXVECTOR3( -1.f, 1.f, 0.f );
	D3DXVec3Normalize( (D3DXVECTOR3*)&light0.Direction, &vecDir0 );
	light0.Range = 1000.f;


	D3DXVECTOR3 vecDir1;
	D3DLIGHT9 light1;
	ZeroMemory( &light1, sizeof( D3DLIGHT9 ) );
	light1.Type = D3DLIGHT_DIRECTIONAL;
	light1.Diffuse.r = 1.f;
	light1.Diffuse.g = 1.f;
	light1.Diffuse.b = 1.f;
	vecDir1 = D3DXVECTOR3( 1.f, 1.f, 0.f );
	D3DXVec3Normalize( (D3DXVECTOR3*)&light1.Direction, &vecDir1 );
	light1.Range = 1000.f;

	g_D3dDevice->SetLight( 0, &light0 );
	g_D3dDevice->SetLight( 1, &light1 );

	switch ( lightNum )
	{
		case 1:
			g_D3dDevice->LightEnable( 0, TRUE );
			g_D3dDevice->LightEnable( 1, FALSE );
			break;
		case 2:
			g_D3dDevice->LightEnable( 0, FALSE );
			g_D3dDevice->LightEnable( 1, TRUE );
			break;
		default:
			break;
	}

	g_D3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
	g_D3dDevice->SetRenderState( D3DRS_AMBIENT, 0x00808080 );
}


//////////////////////////////////////////////////////////////////////////
//render를 pre - main - post renderring으로 변경
//pre에서는 matrix, view, light에 대한 setting 진행
//main에서는 mesh object에 대한 직접 rendering 진행
//post에서는 pre에서 설정한 setting 초기화
//////////////////////////////////////////////////////////////////////////
YAMANGDXDLL_API bool PreRendering( float moveX, float moveY, float moveZ )
{
	if ( NULL == g_D3dDevice )
	{
		return false;
	}

	g_D3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB( 30, 10, 10 ), 1.0f, 0 );

	bool flag = false;
	//렌더 방어코드
	//pre rendering 단계에서 진행되지 않으면 향후 render 모두 실패
	if ( SUCCEEDED( g_D3dDevice->BeginScene() ) )
	{
		SetupTranslateMatrices( moveX, moveY, moveZ );
		ViewSetting();
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

		flag = true;
	}

	return flag;
}

/*
YAMANGDXDLL_API bool PreRendering( D3DXMATRIXA16* matView )
{
	if ( NULL == g_D3dDevice )
	{
		return false;
	}

	g_D3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB( 30, 10, 10 ), 1.0f, 0 );

	bool flag = false;
	//렌더 방어코드
	//pre rendering 단계에서 진행되지 않으면 향후 render 모두 실패
	if ( SUCCEEDED( g_D3dDevice->BeginScene() ) )
	{
		SetMatrix( matView, true );
		D3DXMATRIXA16 matProj;
		D3DXMatrixPerspectiveFovLH( &matProj, D3DX_PI / 2, 1.0f, 1.0f, 100.0f );
		g_D3dDevice->SetTransform( D3DTS_PROJECTION, &matProj );

		D3DXMATRIXA16 mat;
		D3DXMatrixIdentity( &mat );
		g_D3dDevice->SetTransform( D3DTS_WORLD, &mat );

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

		flag = true;
	}

	return flag;
}
*/
YAMANGDXDLL_API void Rendering( MESHOBJECT* inputVal, float moveX, float moveY, float moveZ )
{
	D3DXMATRIXA16 thisMatrix;

	D3DXMatrixTranslation( &thisMatrix, moveX, moveY, moveZ );
	SetMatrix( &thisMatrix );

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

#ifdef _PRINT_CONSOLE
	Logger::Release();
#endif
}

YAMANGDXDLL_API void SetMatrix( D3DXMATRIXA16* matrix, bool cameraSet /*= false */ )
{
	if ( cameraSet == true )
	{
		g_D3dDevice->SetTransform( D3DTS_VIEW, matrix );

		//D3DXMATRIXA16 matProj;
		//D3DXMatrixPerspectiveFovLH( &matProj, D3DX_PI / 2, 1.0f, 1.0f, 100.0f );
		//g_D3dDevice->SetTransform( D3DTS_PROJECTION, &matProj );
	}
	else
	{
		g_D3dDevice->MultiplyTransform( D3DTS_WORLD, matrix );
	}
}


// 내보낸 변수의 예제입니다.
// YAMANGDXDLL_API int nyaMangDxDll=0;