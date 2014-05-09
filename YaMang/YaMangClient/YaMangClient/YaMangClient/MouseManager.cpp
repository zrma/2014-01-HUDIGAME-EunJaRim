#include "stdafx.h"
#include "MouseManager.h"
#include "MainWindow.h"

#include "YaMangDxDll.h"
#include "SceneManager.h"
#include "ScenePlay.h"

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

	if ( isclicked ) //버튼 다운 시
	{
		float pickedX = 0;
		float pickedZ = 0;

		Scene* scene = SceneManager::GetInstance()->GetNowScene();

		if ( typeid( ScenePlay ) != typeid( *scene ) )
		{
			return;
		}

		CalcPickingRay( m_MousePosition.X, m_MousePosition.Y );
		TransPickedTriangle( 1, &pickedX, &pickedZ );

		static_cast<ScenePlay*>(scene)->SearchCorpsIdByPosition( pickedX, pickedZ );
		Log( "[ %f, %f ] 피킹 중 \n", pickedX, pickedZ );
		

		//드래그 시작 포인트 저장 ; 첫 DOWN시 한번만 실행되므로
		SetDragStartPoint(m_MousePosition.X, m_MousePosition.Y);
	}
	else //버튼 업시
	{
		//두 점 사이가 어느정도 떨어져 있을 경우 드래그 처리
		if ( GetDistanceBetweenCOORD(m_MousePosition, m_PressedMousePosition) <= 3.f )
			return;

		SetLeftDrag();
	}
}

void MouseManager::SetRightClick(bool isclicked)
{
	m_IsRightClicked = isclicked; 

	if (isclicked) //버튼 다운 시
	{
		//오른쪽 마우스 클릭

		//드래그 시작 포인트 저장 ; 첫 DOWN시 한번만 실행되므로
		SetDragStartPoint(m_MousePosition.X, m_MousePosition.Y);
	}
	else // 버튼 업 시
	{
		//두 점 사이가 어느정도 떨어져 있을 경우 드래그 처리
		if ( GetDistanceBetweenCOORD(m_MousePosition, m_PressedMousePosition) <= 3.f )
			return;

		SetRightDrag();
	}
}

void MouseManager::SetLeftDrag()
{

}

void MouseManager::SetRightDrag()
{

}

double MouseManager::GetDistanceBetweenCOORD( COORD C1, COORD C2 )
{
	int distanceX = C1.X - C2.X;
	int distanceY = C1.Y - C2.Y;

	double ret = sqrt(distanceX*distanceX + distanceY*distanceY);

	printf_s("%fl\n", ret);
	return ret;
}

