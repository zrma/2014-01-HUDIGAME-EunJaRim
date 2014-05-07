#include "stdafx.h"
#include "MouseManager.h"
#include "MainWindow.h"

MouseManager::MouseManager()
{
	RECT rect;
	GetWindowRect( MainWindow::GetInstance()->Window(), &rect );
	m_Boundary.X = static_cast<SHORT>( rect.right - rect.left );
	m_Boundary.Y = static_cast<SHORT>( rect.bottom - rect.top );

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

	SetCursorPos( m_WndXPos, m_WndYPos );

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

