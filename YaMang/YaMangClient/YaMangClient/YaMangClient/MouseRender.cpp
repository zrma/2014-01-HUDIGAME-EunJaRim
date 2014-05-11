#include "stdafx.h"
#include "MouseRender.h"
#include "YaMangDxDll.h"
#include "MouseManager.h"
#include "ResourceManager.h"


MouseRender::MouseRender()
{
	Init();
}


MouseRender::~MouseRender()
{
}

void MouseRender::Init()
{
	if ( ResourceManager::GetInstance()->IsCursorReady() )
	{
		SetCursorRenderType( CURSOR_TEXTURE_BASIC );
		MouseManager::GetInstance()->SetGameCursorMode( true );

		m_IsCursorReady = true;
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

void MouseRender::ChangeCursorRenderType()
{
	//왼쪽 클릭 상태만 우선 테스트
	if ( MouseManager::GetInstance()->IsLeftClicked() )
	{
		if ( m_CursorRenderType != CURSOR_TEXTURE_CLICK )
		{
			m_CursorRenderType = CURSOR_TEXTURE_CLICK;
			ChangeCursorImage( CURSOR_TEXTURE_CLICK );
		}
	}
	else
	{
		if ( m_CursorRenderType != CURSOR_TEXTURE_BASIC )
		{
			m_CursorRenderType = CURSOR_TEXTURE_BASIC;
			ChangeCursorImage( CURSOR_TEXTURE_BASIC );
		}
	}
}

