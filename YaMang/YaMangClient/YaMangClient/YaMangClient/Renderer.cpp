#include "stdafx.h"
#include "YaMangDxDll.h"
#include "Renderer.h"
#include "MainWindow.h"
#include "ResourceManager.h"
#include "TextManager.h"
#include "CameraController.h"
#include "QuadTree.h"
#include "MapManager.h"

const wchar_t* FONT_TYPE = L"맑은 고딕";

Renderer::Renderer()
{
}

Renderer::~Renderer()
{
}

bool Renderer::Init()
{
	RECT rect;
	GetWindowRect( MainWindow::GetInstance()->Window(), &rect );
	LONG width = rect.right - rect.left;
	LONG height = rect.bottom - rect.top;

	if ( S_OK == InitD3D( MainWindow::GetInstance( )->Window( ), width, height ) )
	{
		m_Result = true;
		m_PrevTime = timeGetTime();
	}

	return m_Result;
}

void Renderer::Destroy()
{
	if ( m_Result )
	{
		D3DCleanUp( );
	}
}

void Renderer::RenderBegin()
{
	if ( m_Result )
	{
		m_DPCall = 0;
		m_IsReady = PreRendering( );
	}
}

void Renderer::RenderEnd()
{
	if ( m_Result && m_IsReady )
	{
		UINT nowTime = timeGetTime();
		if ( m_PrevTime + 1000 < nowTime )
		{
			m_PrevTime = nowTime;
			m_FPS = m_Frame;
			m_Frame = 0;
		}

		wchar_t ws[100] = { 0, };
		wsprintf( ws, L"FPS : %d,  DPCall : %d,  Map : %d ", m_FPS, m_DPCall, m_HeightMapVertis );
		//TextManager::GetInstance( )->RegistText( TEXT_FPS, ws, 20, 20, SCENE_PLAY );
		m_Frame++;

		PostRendering( );
		m_IsReady = false;
	}
}

void Renderer::RenderMesh( MESHOBJECT* mesh )
{
	if ( mesh && m_IsReady )
	{	
		++m_DPCall;
		Rendering( mesh );
	}
}

void Renderer::RenderMap( QuadTree* quadTree, Frustum* frustum, CUSTOMVERTEX* heightMap, float ratioOfLOD )
{
	if ( ResourceManager::GetInstance()->IsMapReady() )
	{
		LPDWORD index = nullptr;

		if ( !quadTree || !frustum || !heightMap || FAILED( PreRenderHeightWithMapQuadTree( &index ) ))
		{
			RenderHeightMap();
		}
		else
		{
			m_HeightMapVertis = quadTree->GenerateIndex( index, heightMap, frustum, ratioOfLOD );
			RenderHeightMapWithQuadTree( m_HeightMapVertis, m_IsHeightMapWire );
		}

		RenderSkyBox( CameraController::GetInstance()->GetEyePoint() );
	}
}

void Renderer::RenderBillboard( int id, char flag )
{
	DrawBillboardByTexture( id, flag );
}

void Renderer::GetHeightMapSize( DWORD* width, DWORD* height ) const
{
	GetHeightMapSizeForQuadTree( width, height );
	MapManager::GetInstance()->SetHeightMapSize( *width, *height );
}

CUSTOMVERTEX* Renderer::GetHeightMap() const
{
	CUSTOMVERTEX* heightMap;
	GetHeightMapForQuadTree( &heightMap );
	MapManager::GetInstance()->SetHeightMap( heightMap );

	return heightMap;
}

void Renderer::SetViewMatrix( D3DXMATRIXA16& matrix ) const
{
	SetCameraMatrix( &matrix );
}

void Renderer::SetWorldMatrix( D3DXMATRIXA16& matrix ) const
{
	SetWorldAreaMatrix( &matrix );
}

void Renderer::ResizeWindow( LONG width, LONG height )
{
	if ( m_Result )
	{
		SetAspectRatio( width, height );
	}
}

void Renderer::WriteText( LPCWSTR text, float left, float top, int RGB_R /*= 255*/, int RGB_G /*= 255*/, int RGB_B /*= 255*/, float right /*= 0*/, float bottom /*= 0 */ ) const
{
	if ( m_Result )
	{
		RenderText( text, left, top, RGB_R, RGB_G, RGB_B, right, bottom );
	}
}

D3DXVECTOR3 Renderer::GetPickedPointOfCenter() const
{
	RECT rect;
	GetWindowRect( MainWindow::GetInstance()->Window(), &rect );
	SHORT centerOfScreenX = static_cast<SHORT>( ( rect.right - rect.left ) / 2 );
	SHORT centerOfScreenY = static_cast<SHORT>( ( rect.bottom - rect.top ) / 2 );

	CalcPickingRay( centerOfScreenX, centerOfScreenY );

	float pickedX = 0;
	float pickedZ = 0;
	if ( S_FALSE != ( ResourceManager::GetInstance()->IsMapForQuadTreeReady() && TransPickedTriangleQuadTree( &pickedX, &pickedZ ) ) || TransPickedTriangle( &pickedX, &pickedZ ) )
	{
		// Log( "피킹 실패하였으므로 다시 피킹합니다. %d \n", rect.bottom - rect.top );
		CalcPickingRay( centerOfScreenX, rect.bottom - rect.top );
		if ( ResourceManager::GetInstance()->IsMapForQuadTreeReady() )
		{
			TransPickedTriangleQuadTree( &pickedX, &pickedZ );
		}
		else
		{
			TransPickedTriangle( &pickedX, &pickedZ );
		}
	}
	// Log( "[%d %d] -> [%f, %f] 으로 피킹 중 \n", centerOfScreenX, centerOfScreenY, pickedX, pickedZ );

	D3DXVECTOR3 returnPoint = { pickedX, 0, pickedZ };
	return returnPoint;
}

void Renderer::SetShader( bool mode )
{
	SetEffect( mode );
}

bool Renderer::CreateDevice( HWND hWnd, long width, long height )
{
	if ( nullptr == ( m_D3D = Direct3DCreate9( D3D_SDK_VERSION ) ) )
	{
		// 오류
		MessageBox( NULL, L"Failed To Create D3D Factory", L"Renderer", MB_OK );
		return false;
	}

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory( &d3dpp, sizeof( d3dpp ) );
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	D3DCAPS9 caps;
	DWORD dwVSProcess;
	m_D3D->GetDeviceCaps( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps );

	// 지원하는 정점쉐이더 버전이 1.0이하라면 SW쉐이더를, 1.0이상이면 HW쉐이더를 생성한다.
	dwVSProcess =
		( caps.VertexShaderVersion < D3DVS_VERSION( 1, 0 ) ) ? D3DCREATE_SOFTWARE_VERTEXPROCESSING : D3DCREATE_HARDWARE_VERTEXPROCESSING;

	if ( FAILED( m_D3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
		hWnd, dwVSProcess, &d3dpp, &m_D3dDevice ) ) )
	{
		MessageBox( NULL, L"Failed To Create D3D Device", L"Renderer", MB_OK );
		return false;
	}

	m_D3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
	m_D3dDevice->SetRenderState( D3DRS_ZENABLE, TRUE );

	m_StartWidth = static_cast<float>( width );
	m_StartHeight = static_cast<float>( height );

	SetAspectRatio( width, height );

	return true;
}

void Renderer::DestroyDevice()
{
	SafeRelease( m_D3dDevice );
	SafeRelease( m_D3D );
}

bool Renderer::CreateFont()
{
	if ( !m_D3dDevice )
	{
		MessageBox( NULL, L"Try To Create Font Device Before Create D3D Device ", L"Renderer", MB_OK );
		return false;
	}
	
	//텍스트 출력 자원 초기화
	if ( FAILED( D3DXCreateFont(
		m_D3dDevice						// Device 객체 
		, m_FontHeight					// 폰트 높이 
		, m_FontWidth					// 폰트 길이 
		, FW_NORMAL						// 폰트 굵기 (기본 FW_BOLD) 
		, m_FontMipLevel				// 밉레벨 
		, false							// 폰트 기울기 켜기/끄기 설정 
		, DEFAULT_CHARSET				// 문자셋 
		, OUT_DEFAULT_PRECIS			// 출력 정확도 설정 
		, DEFAULT_QUALITY				// 퀄리티 설정 
		, DEFAULT_PITCH | FF_DONTCARE	// 피치 설정 
		, FONT_TYPE						// 글꼴 설정 
		, &m_Font						// 초기화할 Font 
		) ) )
	{
		MessageBox( NULL, L"Failed To Create Font Device", L"Renderer", MB_OK );
		return false;
	}
	
	D3DXCreateSprite( m_D3dDevice, &m_Sprite ); // 스프라이트 초기화 

	return true;
}

void Renderer::DestroyFont()
{
	SafeRelease( m_Sprite );
	SafeRelease( m_Font );
}