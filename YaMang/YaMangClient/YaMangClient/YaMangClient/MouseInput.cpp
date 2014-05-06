#include "stdafx.h"
#include "MouseInput.h"

MouseInput::MouseInput()
{
	m_MousePosition.p_XPos = &m_XPos;
	m_MousePosition.p_YPos = &m_YPos;

	m_PressedMousePosition.p_XPos = &m_PressedXPos;
	m_PressedMousePosition.p_YPos = &m_PressedYPos;

	m_WndMousePos.p_XPos = &m_WndXPos;
	m_WndMousePos.p_YPos = &m_WndYPos;
}


MouseInput::~MouseInput()
{
}

