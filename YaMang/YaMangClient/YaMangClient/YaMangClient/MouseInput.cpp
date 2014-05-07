#include "stdafx.h"
#include "MouseInput.h"

MouseInput::MouseInput()
{
	m_MousePosition.p_XPos = &m_XPos;
	m_MousePosition.p_YPos = &m_YPos;

	m_PressedMousePosition.p_XPos = &m_PressedXPos;
	m_PressedMousePosition.p_YPos = &m_PressedYPos;
}


MouseInput::~MouseInput()
{
}

void MouseInput::MoveMousePosition(int x, int y)
{
	//printf("now WndMouse Pos: %d, %d \n", x, y);
	//printf("const WndMouse Pos: %d, %d \n", m_WndXPos, m_WndYPos);
	SetCursorPos(m_WndXPos, m_WndYPos);

	if ((x + m_WndLocationX) > m_WndXPos)
	{
		m_XPos += ((x + m_WndLocationX) - m_WndXPos);
	}
	if ((x + m_WndLocationX) < m_WndXPos)
	{
		m_XPos -= (m_WndXPos - (x + m_WndLocationX));
	}
	if ((y + m_WndLocationY) > m_WndYPos)
	{
		m_YPos += ((y + m_WndLocationY) - m_WndYPos);
	}
		
	if ((y + m_WndLocationY) < m_WndYPos)
	{
		m_YPos -= (m_WndYPos - (y + m_WndLocationY));
	}
	
	//일단 창밖으로 나가는거 값 때려박음
	if (m_XPos >= 1280)
		m_XPos = 1280;
	if (m_YPos >= 720)
		m_YPos = 720;
	if (m_YPos <= 0)
		m_YPos = 0;
	if (m_XPos <= 0)
		m_XPos = 0;
}

