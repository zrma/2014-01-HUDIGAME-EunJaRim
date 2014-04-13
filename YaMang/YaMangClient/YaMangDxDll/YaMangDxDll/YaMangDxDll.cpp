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
//Height Map 생성용 공용 자원
//////////////////////////////////////////////////////////////////////////
LPDIRECT3DVERTEXBUFFER9 g_VertexBuffer = nullptr;
LPDIRECT3DINDEXBUFFER9 g_IdxBuffer = nullptr;

LPDIRECT3DTEXTURE9 g_TexHeight = nullptr;
LPDIRECT3DTEXTURE9 g_TexDiffuse = nullptr;

DWORD g_XHeight = 0;
DWORD g_ZHeight = 0;

//////////////////////////////////////////////////////////////////////////
//텍스트 출력에 필요한 자원
//////////////////////////////////////////////////////////////////////////
ID3DXFont*		g_Font = nullptr;		// 글자를 그릴 폰트 스타일 객체
ID3DXSprite*	g_Sprite = nullptr;		// 폰트를 그릴 스프라이트 객체 

struct CUSTOMVERTEX
{
	D3DXVECTOR3 vertexPoint;
	D3DXVECTOR3 vertexNormal;
	D3DXVECTOR2 vertexTexturePoint;
};

#define D3DFVF_CUSTOMVERTEX ( D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1)

struct MYINDEX
{
	WORD _0, _1, _2;
};


//////////////////////////////////////////////////////////////////////////
//input args: 윈도우 핸들
//향후 공용으로 사용할 D3D, D3DDevice에 대해 초기화 진행
//프로그램에서 1회만 실행 후 Process 종료까지 사용
//참고 URL : http://www.delmadang.com/community/bbs_view.asp?bbsNo=17&indx=426040 
//////////////////////////////////////////////////////////////////////////
YAMANGDXDLL_API HRESULT InitD3D( HWND hWnd )
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

	if ( FAILED( g_D3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, 
		hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &g_D3dDevice ) ) )
	{
		MessageBox( NULL, L"Could not CreateDevice", L"YaMang.exe", MB_OK );
		return E_FAIL;
	}

	g_D3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
	g_D3dDevice->SetRenderState( D3DRS_ZENABLE, TRUE );

	//텍스트 출력 자원 초기화
	D3DXCreateFont(
		g_D3dDevice						// Device 객체 
		, 30							// 폰트 높이 
		, 0								// 폰트 길이 
		, FW_NORMAL						// 폰트 굵기 (기본 FW_BOLD) 
		, 1								// 밉레벨 
		, false							// 폰트 기울기 켜기/끄기 설정 
		, DEFAULT_CHARSET				// 문자셋 
		, OUT_DEFAULT_PRECIS			// 출력 정확도 설정 
		, DEFAULT_QUALITY				// 퀄리티 설정 
		, DEFAULT_PITCH | FF_DONTCARE	// 피치 설정 
		, L"맑은 고딕"					// 글꼴 설정 
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

void SetupTranslateMatrices()
{
	D3DXMATRIXA16 matWorld;
	D3DXMatrixIdentity( &matWorld );
	g_D3dDevice->SetTransform( D3DTS_WORLD, &matWorld );

	//x, y, z축 입력 값에 대해 이동 처리
	D3DXMATRIXA16 thisMatrix;
	D3DXMatrixTranslation( &thisMatrix, 0, 0, 0 );
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
YAMANGDXDLL_API bool PreRendering()
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
		SetupTranslateMatrices();
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
HRESULT InitVertexBuffer( HWND hWnd )
{
	D3DSURFACE_DESC ddsd;
	D3DLOCKED_RECT d3drc;

	g_TexHeight->GetLevelDesc( 0, &ddsd );
	g_XHeight = ddsd.Width;
	g_ZHeight = ddsd.Height;

	if ( FAILED( g_D3dDevice->CreateVertexBuffer( ddsd.Width*ddsd.Height*sizeof( CUSTOMVERTEX ), 0, D3DFVF_CUSTOMVERTEX, D3DPOOL_DEFAULT, &g_VertexBuffer, NULL ) ) )
	{
		MessageBox( NULL, L"Fail in Creating VertexBuffer", L"YaMang.exe", MB_OK );
		return E_FAIL;
	}

	//surface lock
	//확인만 하고 쓸일은 없으므로 readonly
	g_TexHeight->LockRect( 0, &d3drc, NULL, D3DLOCK_READONLY );

	VOID* vertices;
	if ( FAILED( g_VertexBuffer->Lock( 0, g_XHeight*g_ZHeight*sizeof( CUSTOMVERTEX ), (void**) &vertices, 0 ) ) )
	{
		MessageBox( NULL, L"Fail in lock VertexBuffer", L"YaMang.exe", MB_OK );
		return E_FAIL;
	}


	//Vertex 구조체 채우기
	CUSTOMVERTEX vertex;
	CUSTOMVERTEX* vertexPointer = (CUSTOMVERTEX*) vertices;

	for ( DWORD z = 0; z < g_ZHeight; ++z )
	{
		for ( DWORD x = 0; x < g_XHeight; ++x )
		{
			vertex.vertexPoint.x = (float) x - g_XHeight / 2.0f;
			vertex.vertexPoint.z = -( (float) z - g_ZHeight / 2.0f );
			vertex.vertexPoint.y = ( (float) ( *( (LPDWORD) d3drc.pBits + x + z*( d3drc.Pitch / 4 ) ) & 0x000000ff ) ) / 16.f;

			//normal 값이고
			//0,0,0 기준으로 각 지점의 normal 값을 계산
			vertex.vertexNormal.x = vertex.vertexPoint.x;
			vertex.vertexNormal.y = vertex.vertexPoint.y;
			vertex.vertexNormal.z = vertex.vertexPoint.z;

			//단위 벡터로 만드는 것
			//정규화 벡터로 변경하는 연산
			D3DXVec3Normalize( &vertex.vertexNormal, &vertex.vertexNormal );

			vertex.vertexTexturePoint.x = (float) x / ( g_XHeight - 1 );
			vertex.vertexTexturePoint.y = (float) z / ( g_ZHeight - 1 );
			*vertexPointer++ = vertex;
		}
	}
	g_VertexBuffer->Unlock();

	g_TexHeight->UnlockRect( 0 );

	return S_OK;
}

HRESULT InitIdxBuffer( HWND hWnd )
{
	if ( FAILED( g_D3dDevice->CreateIndexBuffer( ( g_XHeight - 1 )*( g_ZHeight - 1 ) * 2 * sizeof( MYINDEX ), 0, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &g_IdxBuffer, NULL ) ) )
	{
		MessageBox( NULL, L"Fail in CreateIndexBuffer", L"YaMang.exe", MB_OK );
		return E_FAIL;
	}

	MYINDEX idx;
	MYINDEX* idxPointer;

	if ( FAILED( g_IdxBuffer->Lock( 0, ( g_XHeight - 1 )*( g_ZHeight - 1 ) * 2 * sizeof( MYINDEX ), (void**) &idxPointer, 0 ) ) )
	{
		MessageBox( NULL, L"Fail in index locking", L"YaMang.exe", MB_OK );
		return E_FAIL;
	}

	for ( DWORD z = 0; z < g_ZHeight - 1; ++z )
	{
		for ( DWORD x = 0; x < g_XHeight - 1; ++x )
		{
			idx._0 = static_cast<WORD>( z*g_XHeight + x );
			idx._1 = static_cast<WORD>( z*g_XHeight + x + 1 );
			idx._2 = static_cast<WORD>( ( z + 1 )*g_XHeight + x );
			*idxPointer++ = idx;
			idx._0 = static_cast<WORD>( ( z + 1 )*g_XHeight + x );
			idx._1 = static_cast<WORD>( z*g_XHeight + x + 1 );
			idx._2 = static_cast<WORD>( ( z + 1 ) *g_XHeight + x + 1 );
			*idxPointer++ = idx;
		}
	}
	g_IdxBuffer->Unlock();

	return S_OK;
}


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
	g_D3dDevice->SetStreamSource( 0, g_VertexBuffer, 0, sizeof( CUSTOMVERTEX ) );
	g_D3dDevice->SetFVF( D3DFVF_CUSTOMVERTEX );

	g_D3dDevice->SetTexture( 0, g_TexDiffuse );

	g_D3dDevice->SetIndices( g_IdxBuffer );
	g_D3dDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, g_XHeight*g_ZHeight, 0, ( g_XHeight - 1 )*( g_ZHeight - 1 ) * 2 );
}

YAMANGDXDLL_API void SetAspectRatio( LONG width, LONG height )
{
	D3DXMATRIXA16 matProj;
	float aspectRatio = static_cast<float>( width ) / static_cast<float>( height );

	D3DXMatrixPerspectiveFovLH( &matProj, D3DX_PI / 2, aspectRatio, 1.0f, 100.0f );
	g_D3dDevice->SetTransform( D3DTS_PROJECTION, &matProj );
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

YAMANGDXDLL_API void RenderTextCleanUp()
{
	if ( g_Font != NULL )
	{
		g_Font->Release();
	}

	if ( g_Sprite != NULL )
	{
		g_Sprite->Release();
	}
}

// 내보낸 변수의 예제입니다.
// YAMANGDXDLL_API int nyaMangDxDll=0;

