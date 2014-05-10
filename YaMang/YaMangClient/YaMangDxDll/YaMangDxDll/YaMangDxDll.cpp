// yaMangDxDll.cpp : DLL 응용 프로그램을 위해 내보낸 함수를 정의합니다.
//

#include "stdafx.h"
#include "yaMangDxDll.h"
#include "Logger.h"
#include <stdio.h>
#include "GlobalVar.h"
#include "InnerResource.h"
#include <WinUser.h>


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
		MessageBox( NULL, L"Could not Create D3D", L"YaMang.DLL", MB_OK );
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
		MessageBox( NULL, L"Could not CreateDevice", L"YaMang.DLL", MB_OK );
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
		MessageBox( NULL, L"Could not find x file", L"YaMang.DLL", MB_OK );
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
					MessageBox( NULL, L"Could not find texture map", L"YaMang.DLL", MB_OK );
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
	g_Ratio = aspectRatio;
	g_Width = static_cast<float>(width);
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

	// 렌더 방어코드
	// pre rendering 단계에서 진행되지 않으면 향후 render 모두 실패
	if ( SUCCEEDED( g_D3dDevice->BeginScene() ) )
	{
		SetupTranslateMatrices();
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
	SetMatrix( &identityMatrix );

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

	if (g_Mesh == nullptr)
	{
		InitGroundMesh(100, 100);
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
	SetMatrix( &viewMatrix , true);

	// 보여주기 위한 땅을 만듬
	//InitGroundMesh(100, 100);
	CreateRawGround(100, 100, 10);
	HeightMapRender();

	//와이어 프레임 해제
	g_D3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

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
	// 텍스트 소멸자 합침
	if ( g_Font != NULL )
	{
		g_Font->Release();
	}

	if ( g_Sprite != NULL )
	{
		g_Sprite->Release();
	}
	
	if ( g_Mesh != NULL )
	{
		g_Mesh->Release();
	}

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

YAMANGDXDLL_API void SetCameraView(float x /* = 0 */, float y /* = 0 */, float z/* = 0 */)
{
	g_EyePoint = { x, y, z };
	g_LookAtPoint.x = x;
	g_LookAtPoint.y = y;
	g_LookAtPoint.z = z + 1;
	D3DXMATRIXA16 mat;
	D3DXMatrixIdentity(&mat);
	D3DXMatrixLookAtLH(&mat, &g_EyePoint, &g_LookAtPoint, &g_UpVector);
	SetMatrix(&mat, true);
}

//////////////////////////////////////////////////////////////////////////
//height map 세계에 오신 것을 환영합니다.
// :)
//////////////////////////////////////////////////////////////////////////


YAMANGDXDLL_API HRESULT HeightMapTextureImport ( HWND hWnd, LPCTSTR heightMap, LPCTSTR mapTexture )
{
	if ( FAILED( D3DXCreateTextureFromFileEx( g_D3dDevice, heightMap, D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, D3DFMT_X8B8G8R8, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &g_TexHeight ) ) )
	{
		MessageBox( NULL, L"Could not find heightMap file", L"YaMang.DLL", MB_OK );
		return E_FAIL;
	}

	if ( FAILED( D3DXCreateTextureFromFile( g_D3dDevice, mapTexture, &g_TexDiffuse ) ) )
	{
		MessageBox( NULL, L"Could not find heightMapTexture file", L"YaMang.DLL", MB_OK );
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

}

YAMANGDXDLL_API void PreSettingForTool()
{
	// 월드 행렬
	D3DXMATRIXA16 world;
	D3DXMatrixIdentity( &world );
	g_D3dDevice->SetTransform( D3DTS_WORLD, &world );


	D3DXVECTOR3 vEyePt(0.f, 40.f, -100.f);
	D3DXVECTOR3 vLookatPt(0.f, 39.5f, -99.0f);
	D3DXVECTOR3 vUpVec(0.f, 1.f, 0.f);

	D3DXMATRIXA16 matView;
	D3DXMatrixLookAtLH( &matView, &vEyePt, &vLookatPt, &vUpVec );
	g_D3dDevice->SetTransform( D3DTS_VIEW, &matView );

	// 프로젝션 행렬
	SetAspectRatio( 729, 588 );	
}

YAMANGDXDLL_API void HeightMapRender()
{
	D3DXMATRIXA16 worldMatrix;
	g_D3dDevice->GetTransform(D3DTS_WORLD, &worldMatrix);


// 	Log("%f, %f, %f, %f \n", worldMatrix._11, worldMatrix._12, worldMatrix._13, worldMatrix._14);
// 	Log("%f, %f, %f, %f \n", worldMatrix._21, worldMatrix._22, worldMatrix._23, worldMatrix._24);
// 	Log("%f, %f, %f, %f \n", worldMatrix._31, worldMatrix._32, worldMatrix._33, worldMatrix._34);
// 	Log("%f, %f, %f, %f \n", worldMatrix._41, worldMatrix._42, worldMatrix._43, worldMatrix._44);
// 	Log("==============================");

	IDirect3DVertexBuffer9* RenderVertexBuffer = nullptr;
	g_Mesh->GetVertexBuffer( &RenderVertexBuffer );

	IDirect3DIndexBuffer9* RenderIndexBuffer = nullptr;
	g_Mesh->GetIndexBuffer( &RenderIndexBuffer );

	// 조명이 들어가면 버텍스 쪽 와이어가 색을 제대로 못 뿌림
	g_D3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
	g_D3dDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME );

	g_D3dDevice->SetStreamSource( 0, RenderVertexBuffer, 0, sizeof( CUSTOMVERTEX ) );
	g_D3dDevice->SetFVF( D3DFVF_CUSTOMVERTEX );

	g_D3dDevice->SetTexture( 0, g_TexDiffuse );
	g_D3dDevice->SetIndices( RenderIndexBuffer );

	g_D3dDevice->DrawIndexedPrimitive( D3DPT_TRIANGLESTRIP, 0, 0, g_XHeight * g_ZHeight, 0, ( g_XHeight - 1 ) * ( g_ZHeight - 1 ) * 2 + ( g_ZHeight - 2 ) * 3 );
}

YAMANGDXDLL_API void InitGroundMesh( int row, int col )
{
	if (g_Mesh != nullptr)
	{
		g_Mesh->Release();
	}
	g_XHeight = col + 1;
	g_ZHeight = row + 1;

	int verticesCount = (g_XHeight) * ( g_ZHeight );
	int faceCount = ( g_XHeight - 1 ) * ( g_ZHeight - 1 ) * 2;

	//mesh를 직접 만들어 보자
	//mesh를 직접 만들게 되면 사실 global vertex buffer와 index buffer가 무의미 하다.

	HRESULT hr = NULL;
	hr = D3DXCreateMeshFVF( faceCount, verticesCount, D3DXMESH_MANAGED | D3DXMESH_32BIT, D3DFVF_CUSTOMVERTEX, g_D3dDevice, &g_Mesh );
	if ( hr != S_OK )
	{
		MessageBox( NULL, L"CreateMesh Failed", L"YaMang.DLL", MB_OK );
		return;
	}
}


YAMANGDXDLL_API void CreateRawGround( int row, int col, float pixelSize )
{
	int verticesCount = (g_XHeight) * ( g_ZHeight );
	
	CUSTOMVERTEX* baseVertex = new CUSTOMVERTEX[verticesCount];
	if ( nullptr == baseVertex )
	{
		assert( false );
	}

	int startIdx = 0;
	CUSTOMVERTEX vPos0;

	vPos0.vertexPoint.x = -1.f * col * pixelSize * 0.5f;
	vPos0.vertexPoint.y = 0.0f;
	vPos0.vertexPoint.z = -1.f * row * pixelSize * 0.5f;
	for ( int z = 0; z <= row; ++z )
	{
		for ( int x = 0; x <= col; ++x )
		{
			baseVertex[startIdx].vertexPoint.x = vPos0.vertexPoint.x + ( pixelSize * x );
			baseVertex[startIdx].vertexPoint.y = 0.f;
			baseVertex[startIdx].vertexPoint.z = vPos0.vertexPoint.z + ( pixelSize * z );
			baseVertex[startIdx].Diffuse = D3DCOLOR_ARGB( 255, 150, 30, 30 );
			++startIdx;
		}
	}
	/*
	//vertex 내용 확인
	for ( int i = 0; i < startIdx; ++i )
	{
		printf_s( "vertex %d: (%f, %f)\n", i, baseVertex[i].vertexPoint.x, baseVertex[i].vertexPoint.z );
	}
	*/

	void *pVertices;
	if ( FAILED( g_Mesh->LockVertexBuffer( 0, &pVertices ) ) )
	{
		if ( baseVertex )
		{
			delete[] baseVertex;
			baseVertex = nullptr;
		}
		MessageBox( NULL, L"Fail in lock VertexBuffer", L"YaMang.DLL", MB_OK );
		return;
	}
	memcpy( pVertices, baseVertex, verticesCount * sizeof( CUSTOMVERTEX ) );
	g_Mesh->UnlockVertexBuffer();



	int indicesCount = g_XHeight * g_ZHeight + ( g_ZHeight - 2 )*( g_XHeight + 1 );
	UINT* baseIndex = new UINT[indicesCount];
	if ( nullptr == baseIndex )
	{
		assert( false );
	}

	//퇴적 삼각형 적용 부분
	startIdx = 0;
	for ( int z = 0; z < row; ++z )
	{
		if ( z % 2 == 0 )
		{
			int x;
			for (  x = 0; x < col+1; ++x )
			{
				UINT a = static_cast<UINT>( x + ( z*g_XHeight ) );
				baseIndex[startIdx++] = a;
				UINT b = static_cast<UINT>( x + ( z*g_XHeight ) + g_XHeight );
				baseIndex[startIdx++] = b;
				//Log( "a: %d | b: %d\n", a, b );
			}
			if ( z != g_ZHeight-2 )
			{
				UINT c = static_cast<UINT>( ( x - 1 ) + ( z*g_XHeight ) + g_XHeight );
				baseIndex[startIdx++] = c;
				//Log( "c: %d \n", c );
			}
		}
		else
		{
			int x;
			for ( x = col; x >=0; --x )
			{
				UINT a = static_cast<UINT>( x + ( z*g_XHeight ) );
				baseIndex[startIdx++] = a;
				UINT b = static_cast<UINT>( x + ( z*g_XHeight ) + g_XHeight );
				baseIndex[startIdx++] = b;
				//Log( "a: %d | b: %d\n", a, b );
			}
			if ( z != g_ZHeight - 2 )
			{
				UINT c = static_cast<UINT>( ( x + 1 ) + ( z*g_XHeight ) + g_XHeight );
				baseIndex[startIdx++] = c;
				//Log( "c: %d \n", c );
			}
		}
	}
	
	/*
	//index buffer에 뭐있나?
	for ( int i = 0; i < startIdx; ++i )
	{
		printf_s( "index %d: %d\n", i, baseIndex[i] );
	}
	*/

	void* pIndices;
	if ( FAILED( g_Mesh->LockIndexBuffer( 0, &pIndices ) ) )
	{
		if ( baseVertex )
		{
			delete[] baseVertex;
			baseVertex = nullptr;
		}
		if ( baseIndex )
		{
			delete[] baseIndex;
			baseIndex = nullptr;
		}
		MessageBox( NULL, L"Fail in lock IndexBuffer", L"YaMang.DLL", MB_OK );
		return;
	}
	memcpy( pIndices, baseIndex, sizeof(UINT) *indicesCount );

	g_Mesh->UnlockIndexBuffer();

	if ( baseVertex )
	{
		delete[] baseVertex;
		baseVertex = nullptr;
	}
	if ( baseIndex )
	{
		delete[] baseIndex;
		baseIndex = nullptr;
	}
}



//////////////////////////////////////////////////////////////////////////
// Picking Ray를 위한 부분
//////////////////////////////////////////////////////////////////////////

YAMANGDXDLL_API void CalcPickingRay( int mouseX, int mouseY )
{
	D3DVIEWPORT9 viewPort;
	D3DXMATRIXA16 projectionMatrix;

	float rayX = 0.f;
	float rayY = 0.f;

	g_D3dDevice->GetViewport( &viewPort );
	g_D3dDevice->GetTransform( D3DTS_PROJECTION, &projectionMatrix );

	rayX = ( ( ( ( ( mouseX - viewPort.X ) * 2.0f / (float) viewPort.Width ) - 1.0f ) ) - projectionMatrix( 2, 0 ) ) / projectionMatrix( 0, 0 );
	rayY = ( ( -( ( ( mouseY - viewPort.Y ) * 2.0f / (float) viewPort.Height ) - 1.0f ) ) - projectionMatrix( 2, 1 ) ) / projectionMatrix( 1, 1 );
	// Log( "Ray x : %f, y : %f\n", rayX, rayY );

	//viewport트랜스, 프로젝션 트랜스 역행
	g_RayOrigin = { 0.f, 0.f, 0.f };
	g_RayDirection = { rayX, rayY, 1.f };
	// Log( "뷰포트, 프로젝션 역행\n" );

	//뷰잉 트랜스 역행
	D3DXMATRIXA16 viewingMatrix;
	g_D3dDevice->GetTransform( D3DTS_VIEW, &viewingMatrix );
	D3DXMatrixInverse( &viewingMatrix, 0, &viewingMatrix );

	D3DXVec3TransformCoord( &g_RayOrigin, &g_RayOrigin, &viewingMatrix );
	D3DXVec3TransformNormal( &g_RayDirection, &g_RayDirection, &viewingMatrix );
	// Log( "뷰잉 좌표 역행\n" );

	//월드 좌표로 역행
	D3DXMATRIXA16 worldMatrix;
	g_D3dDevice->GetTransform( D3DTS_WORLD, &worldMatrix );
	D3DXMatrixInverse( &worldMatrix, 0, &worldMatrix );

	D3DXVec3TransformCoord( &g_RayOrigin, &g_RayOrigin, &worldMatrix );
	D3DXVec3TransformNormal( &g_RayDirection, &g_RayDirection, &worldMatrix );
	// Log( "월드 좌표 역행\n" );
	// Log( "origin: %f,%f,%f\n direction: %f, %f, %f\n", g_RayOrigin.x, g_RayOrigin.y, g_RayOrigin.z, g_RayDirection.x, g_RayDirection.y, g_RayDirection.z );

}

YAMANGDXDLL_API void TransPickedTriangle( int modeSelector, float* pickedX, float* pickedZ )
{
	if ( !( pickedX || pickedZ ) )
	{
		return;
	}

	LPDIRECT3DVERTEXBUFFER9 presentVertexBuffer;
	g_Mesh->GetVertexBuffer( &presentVertexBuffer );

	CUSTOMVERTEX* VerticesStartPoint;
	presentVertexBuffer->Lock( 0, 0, (void**)&VerticesStartPoint, 0 );

	BOOL Hit1 = false;
	BOOL Hit2 = false;
	float dist1 = 0;
	float dist2 = 0;

	int trianglePointA = NULL, trianglePointB = NULL, trianglePointC = NULL, trianglePointD = NULL;

	for ( UINT z = 0; ( z < ( g_ZHeight - 1 ) ) && !( Hit1 | Hit2 ); ++z )
	{
		if ( z % 2 == 0 )
		{
			for ( UINT x = 0; ( x < ( g_XHeight - 1 ) ) && !( Hit1 | Hit2 ); ++x )
			{
				trianglePointA = g_ZHeight*z + x;
				trianglePointB = g_ZHeight*z + ( x + 1 );
				trianglePointC = g_ZHeight*( z + 1 ) + x;
				Hit1 = D3DXIntersectTri( &VerticesStartPoint[trianglePointA].vertexPoint, &VerticesStartPoint[trianglePointB].vertexPoint, &VerticesStartPoint[trianglePointC].vertexPoint, &g_RayOrigin, &g_RayDirection, pickedX, pickedZ, &dist1 );

				trianglePointD = g_ZHeight*( z + 1 ) + ( x + 1 );
				Hit2 = D3DXIntersectTri( &VerticesStartPoint[trianglePointB].vertexPoint, &VerticesStartPoint[trianglePointC].vertexPoint, &VerticesStartPoint[trianglePointD].vertexPoint, &g_RayOrigin, &g_RayDirection, pickedX, pickedZ, &dist2 );
			}
		}
		else
		{
			for ( UINT x = g_ZHeight - 1; ( x > 0 ) && !( Hit1 | Hit2 ); --x )
			{
				trianglePointA = g_ZHeight*z + x;
				trianglePointB = g_ZHeight*z + ( x - 1 );
				trianglePointC = g_ZHeight*( z + 1 ) + x;
				Hit1 = D3DXIntersectTri( &VerticesStartPoint[trianglePointA].vertexPoint, &VerticesStartPoint[trianglePointB].vertexPoint, &VerticesStartPoint[trianglePointC].vertexPoint, &g_RayOrigin, &g_RayDirection, pickedX, pickedZ, &dist1 );

				trianglePointD = g_ZHeight*( z + 1 ) + ( x - 1 );
				Hit2 = D3DXIntersectTri( &VerticesStartPoint[trianglePointB].vertexPoint, &VerticesStartPoint[trianglePointC].vertexPoint, &VerticesStartPoint[trianglePointD].vertexPoint, &g_RayOrigin, &g_RayDirection, pickedX, pickedZ, &dist2 );
			}
		}
	}

	if ( ( Hit1 || Hit2 ) && ( dist1 > 0 || dist2 > 0 ) )
	{
		CUSTOMVERTEX* intersectedVertexBufferStart;
		g_Mesh->LockVertexBuffer( 0, (void**)&intersectedVertexBufferStart );

		CUSTOMVERTEX* pointA;
		CUSTOMVERTEX* pointB;
		CUSTOMVERTEX* pointC;

		if ( Hit1 )
		{
			pointA = &VerticesStartPoint[trianglePointA];
			pointB = &VerticesStartPoint[trianglePointB];
			pointC = &VerticesStartPoint[trianglePointC];
		}
		else
		{
			pointA = &VerticesStartPoint[trianglePointB];
			pointB = &VerticesStartPoint[trianglePointC];
			pointC = &VerticesStartPoint[trianglePointD];
		}

		*pickedX += pointA->vertexPoint.x;
		*pickedZ += pointA->vertexPoint.z;

		modeSelector = static_cast<AreaModeType>( modeSelector );

		switch ( modeSelector )
		{
			case AREA_MODE_NONE:

				break;
			case AREA_MODE_COLOR:
			{
				pointA->Diffuse = D3DCOLOR_ARGB( 255, 30, 100, 100 );
				pointB->Diffuse = D3DCOLOR_ARGB( 255, 30, 100, 100 );
				pointC->Diffuse = D3DCOLOR_ARGB( 255, 30, 100, 100 );
			}
				break;
			case AREA_MODE_HIGHER:

				break;
			case AREA_MODE_LOWER:

				break;
			default:
				break;
		}

		g_Mesh->UnlockVertexBuffer();
	}

	presentVertexBuffer->Unlock();
	presentVertexBuffer->Release();
}

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
	float ratio = (720.0f / g_Width) * g_Ratio;
	D3DXMatrixIdentity(&ratioMat);
	D3DXMatrixScaling(&ratioMat, 1280 / g_Width, ratio, 1);
	g_Sprite->SetTransform(&ratioMat);

	g_Font->DrawText( g_Sprite, text
					  , -1										// 총 문자열 수(그냥 -1 해도 됨.) 
					  , &rt									// 그릴 위치 
					  , DT_NOCLIP								// 옵션 플래그
					  , D3DCOLOR_XRGB( RGB_R, RGB_G, RGB_B )	// 그릴 색 , default로 흰색
					  );
	g_Sprite->End();											// 스프라이트 그리기 끝 
}


YAMANGDXDLL_API HRESULT InitCursor( LPCWSTR cursorImagePath, int cursorPosX, int cursorPosY )
{
	if ( FAILED( D3DXCreateTextureFromFile( g_D3dDevice, cursorImagePath, &g_cursorTex ) ) )
	{
		return E_FAIL;
	}
	if ( FAILED( D3DXCreateSprite( g_D3dDevice, &g_cursorSprite ) ) ) //first parameter is our device, second is a empty sprite variable
	{
		return E_FAIL;
	}

	SetCursorPosition( cursorPosX, cursorPosY ); // 0,0,0으로 초기화

	return S_OK;
}

YAMANGDXDLL_API HRESULT CursorRender( )
{
	if ( g_cursorSprite ) 
	{
		D3DXMATRIXA16 ratioMat;
		
		float ratio = (720.0f / g_Width) * g_Ratio;
		D3DXMatrixIdentity(&ratioMat);
		D3DXMatrixScaling(&ratioMat, 1280 / g_Width, ratio, 1);
		g_cursorSprite->SetTransform(&ratioMat);
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

YAMANGDXDLL_API void SetCursorPosition( int PosX, int PosY )
{
	//Log("Render Cursor Pos %d %d \n",PosX,PosY);
	g_cursorPos.x = static_cast<float>(PosX);
	g_cursorPos.y = static_cast<float>(PosY);
	g_cursorPos.z = 0.0f;
}

//////////////////////////////////////////////////////////////////////////
///Camera 함수
//////////////////////////////////////////////////////////////////////////
YAMANGDXDLL_API void MoveCamera(float x, float y, float z)
{
	D3DXVECTOR3 view = g_LookAtPoint - g_EyePoint;
	D3DXVECTOR3 cross;
	D3DXVec3Cross(&cross, &g_UpVector, &view);
	D3DXVec3Normalize(&view, &view);
	view.y = 0;
	g_EyePoint = g_EyePoint + cross * x + view * z;
	g_LookAtPoint = g_LookAtPoint + cross * x + view * z;

	g_LookAtPoint.y += y;
	g_EyePoint.y += y;

	D3DXMATRIXA16 viewMatrix;
	D3DXMatrixLookAtLH(&viewMatrix, &g_EyePoint, &g_LookAtPoint, &g_UpVector);
	SetMatrix(&viewMatrix, true);
}

YAMANGDXDLL_API void ZoomCamera(float zoom)
{
	D3DXVECTOR3 view = g_LookAtPoint - g_EyePoint;
	D3DXVec3Normalize(&view, &view);
	g_EyePoint += view * zoom;
	g_LookAtPoint += view * zoom;

	D3DXMATRIXA16 viewMatrix;
	D3DXMatrixLookAtLH(&viewMatrix, &g_EyePoint, &g_LookAtPoint, &g_UpVector);
	SetMatrix(&viewMatrix, true);
}
	
// 내보낸 변수의 예제입니다.
// YAMANGDXDLL_API int nyaMangDxDll=0;
