#include "stdafx.h"
#include "MouseRender.h"
#include "YaMangDxDll.h"
#include "MouseManager.h"


MouseRender::MouseRender()
{
	Init();
}


MouseRender::~MouseRender()
{
	Destroy();
}

void MouseRender::Init()
{
	CreateCursor( L"cursor2.png", 
				  MouseManager::GetInstance()->GetMousePositionX(), MouseManager::GetInstance()->GetMousePositionY() );
	MouseManager::GetInstance()->SetGameCursorMode(true);
}

HRESULT MouseRender::CreateCursor( LPCWSTR cursorImagePath, int cursorPosX /*= 0*/, int cursorPosY /*= 0 */ )
{
	if ( FAILED( InitCursor( cursorImagePath, cursorPosX, cursorPosY ) ) )
	{
		m_IsCursorReady = false;
		return E_FAIL;
	}
	m_IsCursorReady = true;
	return S_OK;
}

void MouseRender::DestroyCursor()
{
	if ( m_IsCursorReady )
	{
		CursorCleanUp();
		m_IsCursorReady = false;
	}
}
void MouseRender::Update()
{
	SetGameCursorPos( MouseManager::GetInstance()->GetMousePositionX(), MouseManager::GetInstance()->GetMousePositionY() );
}

void MouseRender::Render() const
{
	if ( m_IsCursorReady )
	{
		RenderCursor();
	}
}

void MouseRender::SetGameCursorPos(int PosX, int PosY)
{
	SetCursorPosition( PosX, PosY );
}

void MouseRender::Destroy()
{
	DestroyCursor();
	MouseManager::Release();
}

