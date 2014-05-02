#include "stdafx.h"
#include "MouseRender.h"
#include "YaMangDxDll.h"


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
	CreateCursor(L"cursor2.png",200,200);
}

HRESULT MouseRender::CreateCursor( LPCWSTR cursorImagePath, float cursorPosX /*= 0*/, float cursorPosY /*= 0 */ )
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
	
}

void MouseRender::Render() const
{
	RenderCursor();
}

void MouseRender::SetCursorPos(float PosX, float PosY)
{
	SetCursorPosition(PosX, PosY);
}

void MouseRender::Destroy()
{
	DestroyCursor();
}

