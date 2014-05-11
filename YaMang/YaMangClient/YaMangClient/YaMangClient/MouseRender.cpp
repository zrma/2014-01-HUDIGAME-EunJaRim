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
	SetCursorRenderType(CursorRenderType::CURSOR_RENDER_BASIC);
	
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
	ChangeCursorRenderType();
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

void MouseRender::ChangeCursorRenderType()
{
	//왼쪽 클릭 상태만 우선 테스트
	if (MouseManager::GetInstance()->IsLeftClicked())
	{
		if (m_CursorRenderType != CursorRenderType::CURSOR_RENDER_CLICK)
		{
			m_CursorRenderType = CursorRenderType::CURSOR_RENDER_CLICK;
			ChangeCursorImage(L"cursor_clicked.png");
		}
	}
	else
	{
		if (m_CursorRenderType != CursorRenderType::CURSOR_RENDER_BASIC)
		{
			m_CursorRenderType = CursorRenderType::CURSOR_RENDER_BASIC;
			ChangeCursorImage(L"cursor2.png");
		}
	}

}

