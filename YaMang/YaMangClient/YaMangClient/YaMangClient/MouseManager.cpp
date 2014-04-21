#include "stdafx.h"
#include "MouseManager.h"
#include "YaMangDxDll.h"


MouseManager::MouseManager()
{
}


MouseManager::~MouseManager()
{
}

HRESULT MouseManager::CreateCursor( LPCWSTR cursorImagePath, float cursorPosX /*= 0*/, float cursorPosY /*= 0 */ )
{
	if ( FAILED( InitCursor( cursorImagePath, cursorPosX, cursorPosY ) ) )
	{
		m_IsCursorReady = false;
		return E_FAIL;
	}
	m_IsCursorReady = true;
	return S_OK;
}

HRESULT MouseManager::RenderCursor()
{
	if ( FAILED( CursorRender() ) )
	{
		return E_FAIL;
	}
	return S_OK;
}

void MouseManager::CleanupCursor()
{
	if ( m_IsCursorReady )
	{
		CursorCleanUp();
		m_IsCursorReady = false;
	}
}

void MouseManager::SetCursorPos( float PosX, float PosY )
{
	SetCursorPosition( PosX, PosY );
}
