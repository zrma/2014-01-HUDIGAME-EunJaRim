#include "stdafx.h"
#include "MouseRender.h"
#include "YaMangDxDll.h"
#include "MouseInput.h"


MouseRender::MouseRender()
{
	Init();
}


MouseRender::~MouseRender()
{
	DestroyCursor();
}

void MouseRender::Init()
{
	CreateCursor(L"cursor2.png",500,500);
	MouseInput::GetInstance()->SetGameCursorMode(true);
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

HRESULT MouseRender::RenderCursor() const
{
	if ( FAILED( CursorRender() ) )
	{
		return E_FAIL;
	}
	return S_OK;
}

void MouseRender::RemoveWndCursor()
{
	if (m_IsCursorReady)
	{
		SetCursor(NULL);
		ShowCursor(false);
	}
}

void MouseRender::Update()
{
	SetGameCursorPos(MouseInput::GetInstance()->GetMousePositionX(), MouseInput::GetInstance()->GetMousePositionY());
}

void MouseRender::Render() const
{
	if (MouseInput::GetInstance()->IsGameCursorMode())
	{
		RenderCursor();
	}
}

void MouseRender::SetGameCursorPos(int PosX, int PosY)
{
	SetCursorPosition(PosX, PosY);
}

void MouseRender::Destroy()
{
	DestroyCursor();
}

