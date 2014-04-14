#include "stdafx.h"
#include "YaMangDxDll.h"
#include "Renderer.h"
#include "MainWindow.h"
#include "CameraController.h"

extern float myPosX;
extern float myPosY;
extern float myPosZ;

Renderer::Renderer()
{
}

Renderer::~Renderer()
{
}

bool Renderer::Init()
{
	if ( S_OK == InitD3D( MainWindow::GetInstance()->Window() ) )
	{
		m_Result = true;

		RECT rect;
		GetWindowRect( MainWindow::GetInstance()->Window(), &rect );
		LONG width = rect.right - rect.left;
		LONG height = rect.bottom - rect.left;

		ResizeWindow( width, height );
	}

	return m_Result;
}

void Renderer::Destroy()
{
	DeleteMap();
	DeleteTextRenderer();

	if ( m_Result )
	{
		D3DCleanUp();
	}
}

void Renderer::RenderBegin()
{
	if ( m_Result )
	{
		m_IsReady = PreRendering();
	}
}

void Renderer::RenderEnd()
{
	if ( m_Result && m_IsReady )
	{
		PostRendering();
		m_IsReady = false;
	}
}

void Renderer::Render( MESHOBJECT* mesh )
{
	if ( mesh && m_IsReady )
	{
		D3DXMATRIXA16 matrix;
		D3DXMatrixIdentity( &matrix );
		D3DXMatrixTranslation( &matrix, myPosX, myPosY + 25, myPosZ );
		
		SetMatrix( &matrix );

		Rendering( mesh );
	}
}

void Renderer::RenderMap()
{
	if ( m_IsMapReady )
	{
		HeightMapRender();
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
		HeightMapCleanup();
	}

	m_IsMapReady = false;
}

bool Renderer::CreateMesh( LPCTSTR& fileName, MESHOBJECT* mesh )
{
	if ( S_OK == InitGeometry( MainWindow::GetInstance()->Window(), fileName, mesh ) )
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
	SetMatrix( &(CameraController::GetInstance()->GetMatrix()), true );
}

void Renderer::ResizeWindow( LONG width, LONG height )
{
	SetAspectRatio( width, height );
}

void Renderer::DeleteTextRenderer()
{
	// 방어코드 이따 추가
	// 펜이 dll 안에 device랑 같이 init되서
	RenderTextCleanUp();
}
