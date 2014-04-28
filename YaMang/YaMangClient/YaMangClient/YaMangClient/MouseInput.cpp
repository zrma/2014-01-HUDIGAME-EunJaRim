#include "stdafx.h"
#include "MouseInput.h"

MouseInput::MouseInput()
{
	m_MousePosition.p_XPos = &m_xPos;
	m_MousePosition.p_YPos = &m_yPos;

	m_PressedMousePosition.p_XPos = &m_PressedXPos;
	m_PressedMousePosition.p_YPos = &m_PressedYPos;
}


MouseInput::~MouseInput()
{
}

