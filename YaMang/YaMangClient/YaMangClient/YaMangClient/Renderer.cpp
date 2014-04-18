#include "stdafx.h"
#include "YaMangDxDll.h"
#include "Renderer.h"
#include "MainWindow.h"

Renderer::Renderer()
{
}

Renderer::~Renderer()
{
}

bool Renderer::Init()
{
	if ( S_OK == InitD3D( MainWindow::GetInstance( )->Window( ) ) )
	{
		m_Result = true;

		RECT rect;
		GetWindowRect( MainWindow::GetInstance()->Window(), &rect );
		LONG width = rect.right - rect.left;
		LONG height = rect.bottom - rect.left;

		m_PrevTime = timeGetTime();
	}

	return m_Result;
}

void Renderer::Destroy()
{
	DeleteMap();

	if ( m_Result )
	{
		D3DCleanUp( );
	}
}

void Renderer::RenderBegin()
{
	if ( m_Result )
	{
		m_IsReady = PreRendering( );
	}
}

void Renderer::RenderEnd()
{
	if ( m_Result && m_IsReady )
	{
		PostRendering( );
		m_IsReady = false;
	}
}

void Renderer::Render( MESHOBJECT* mesh )
{
	if ( mesh && m_IsReady )
	{	
// 		char szDebug[64] = { 0, };
// 		sprintf_s(szDebug, "matrixSize : %d \n", sizeof(matrix));
// 		OutputDebugStringA(szDebug);

		Rendering( mesh );
	}

	UINT nowTime = timeGetTime();
	if ( m_PrevTime + 1000 < nowTime )
	{
		m_PrevTime = nowTime;
		m_FPS = m_Frame;
		m_Frame = 0;	
	}

	wchar_t ws[100] = { 0, };
	wsprintf( ws, L"FPS : %d", m_FPS );
	RenderText( ws, 20, 40 );
	m_Frame++;
}

void Renderer::RenderMap()
{
	if ( m_IsMapReady )
	{
		HeightMapRender( );
	}
}

bool Renderer::CreateMap( LPCTSTR& heightMapFileName, LPCTSTR& textureFileName )
{
	m_IsMapReady = ( S_OK == HeightMapTextureImport(
		MainWindow::GetInstance()->Window(), heightMapFileName, textureFileName ) );

	return m_IsMapReady;
}

void Renderer::DeleteMap()
{
	if ( m_IsMapReady )
	{
		HeightMapCleanup( );
	}

	m_IsMapReady = false;
}

bool Renderer::CreateMesh( LPCTSTR& fileName, MESHOBJECT* mesh )
{
	if ( S_OK == InitGeometry( MainWindow::GetInstance( )->Window( ), fileName, mesh ) )
	{
		return true;
	}
	return false;
}

void Renderer::DeleteMesh( MESHOBJECT* mesh )
{
	if ( mesh )
	{
		MeshObjectCleanUp( mesh );
	}
}

void Renderer::SetCamera( D3DXMATRIXA16& matrix )
{
	SetMatrix( &matrix, true );
}

void Renderer::ResizeWindow( LONG width, LONG height )
{
	if ( m_Result )
	{
		SetAspectRatio( width, height );
	}
}

void Renderer::WriteText( LPCWSTR text, float left, float top, int RGB_R /*= 255*/, int RGB_G /*= 255*/, int RGB_B /*= 255*/, float right /*= 0*/, float bottom /*= 0 */ )
{
	if ( m_Result )
	{
		RenderText( text, left, top, RGB_R, RGB_G, RGB_B, right, bottom );
	}
}

D3DVIEWPORT9 Renderer::GetViewPort()
{
	D3DVIEWPORT9	viewPort;
	GetViewPort9( &viewPort );

	return viewPort;
}

D3DXMATRIXA16 Renderer::GetProjMatrix()
{
	D3DXMATRIXA16	projMatrix;
	GetD3DProjMatrix( &projMatrix );

	return projMatrix;
}

D3DXMATRIXA16 Renderer::GetViewMatrix()
{
	D3DXMATRIXA16	viewMatrix;
	GetD3DViewMatrix( &viewMatrix );

	return viewMatrix;
}

