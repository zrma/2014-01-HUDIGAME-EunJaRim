#include "stdafx.h"
#include "YaMangDxDll.h"
#include "Renderer.h"
#include "MainWindow.h"
#include "CameraController.h"

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
		D3DXMATRIXA16 matrix;
		
		char szDebug[64] = { 0, };
		sprintf_s(szDebug, "matrixSize : %d \n", sizeof(matrix));
		OutputDebugStringA(szDebug);

		D3DXMatrixIdentity( &matrix );
		
		SetMatrix( &matrix );

		Rendering( mesh );
	}
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

void Renderer::SetCamera()
{
	SetMatrix( &( CameraController::GetInstance( )->GetMatrix( ) ), true );
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

