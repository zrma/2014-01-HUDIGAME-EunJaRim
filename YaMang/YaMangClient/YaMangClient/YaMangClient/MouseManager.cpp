#include "stdafx.h"
#include "MouseManager.h"
#include "MainWindow.h"

#include "YaMangDxDll.h"

MouseManager::MouseManager()
{
	RECT rect;
	GetWindowRect( MainWindow::GetInstance()->Window(), &rect );
	m_Boundary.X = static_cast<SHORT>( rect.right - rect.left );
	m_Boundary.Y = static_cast<SHORT>( rect.bottom - rect.top );

	m_WndLocationX = static_cast<int>(rect.left);
	m_WndLocationY = static_cast<int>(rect.top);

	m_WndXPos = m_PressedMousePosition.X = m_MousePosition.X = m_Boundary.X / 2;
	m_WndYPos = m_PressedMousePosition.Y = m_MousePosition.Y = m_Boundary.Y / 2;
}

MouseManager::~MouseManager()
{
}

void MouseManager::MoveMousePosition( int x, int y )
{
	if ( !m_GameCursorMod )
	{
		return;
	}

	if ( ( x + m_WndLocationX ) > m_WndXPos )
	{
		m_MousePosition.X += ( ( x + m_WndLocationX ) - m_WndXPos );
	}
	if ( ( x + m_WndLocationX ) < m_WndXPos )
	{
		m_MousePosition.X -= ( m_WndXPos - ( x + m_WndLocationX ) );
	}
	if ( ( y + m_WndLocationY ) > m_WndYPos )
	{
		m_MousePosition.Y += ( ( y + m_WndLocationY ) - m_WndYPos );
	}

	if ( ( y + m_WndLocationY ) < m_WndYPos )
	{
		m_MousePosition.Y -= ( m_WndYPos - ( y + m_WndLocationY ) );
	}

	//일단 창밖으로 나가는거 값 때려박음
	if ( m_MousePosition.X > m_Boundary.X - 35 )
	{
		m_MousePosition.X = m_Boundary.X - 35;
	}
	if ( m_MousePosition.Y > m_Boundary.Y - 35 )
	{
		m_MousePosition.Y = m_Boundary.Y - 35;
	}
	if ( m_MousePosition.X < 3 )
	{
		m_MousePosition.X = 3;
	}
	if ( m_MousePosition.Y < 5 )
	{
		m_MousePosition.Y = 5;
	}
}

void MouseManager::MoveHiddenCursorToCenter()
{
	SetCursorPos( m_WndXPos, m_WndYPos );
}

void MouseManager::MoveHiddenCursorToMousePos()
{
	SetCursorPos( m_MousePosition.X, m_MousePosition.Y );
}

void MouseManager::SetLeftClick( bool isclicked )
{
	m_IsLeftClicked = isclicked;

	if ( isclicked )
	{
		float pickedX = 0;
		float pickedZ = 0;

		CalcPickingRay( m_MousePosition.X, m_MousePosition.Y );
		TransPickedTriangle( 0, &pickedX, &pickedZ );

		Log( "[ %f, %f ] 피킹 중 \n", pickedX, pickedZ );
	}
}

