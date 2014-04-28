#pragma once
#include "MacroSet.h"

enum class MouseStatus
{
	MOUSE_NONE,
	MOUSE_DOWN,
	MOUSE_PRESSED,
	MOUSE_UP
};

struct MousePos
{
	int* p_XPos;
	int* p_YPos;
};

class MouseInput : public Singleton<MouseInput>
{
public:
	MouseInput();
	~MouseInput();

	MousePos		GetMousePosition() { return m_MousePosition; }
	int				GetMousePositionX() { return m_xPos; }
	int				GetMousePositionY() { return m_yPos; }
	void			SetMousePosition(int xPos, int yPos) { m_xPos = xPos; m_yPos = yPos; }

	MouseStatus		GetMouseStatus() { return m_MouseStatus; }
	void			SetMouseStatus(MouseStatus mouseStatus) { m_MouseStatus = mouseStatus; }

	void			MoveMousePosition(int x, int y) { m_xPos += x; m_yPos += y; }

private:
	//현재 마우스 좌표 저장
	int				m_xPos = 200;
	int				m_yPos = 200;
	MousePos		m_MousePosition;

	//드래그 시작 마우스 좌표 저장
	int				m_PressedXPos = 200;
	int				m_PressedYPos = 200; 
	MousePos		m_PressedMousePosition;

	MouseStatus		m_MouseStatus = MouseStatus::MOUSE_NONE;
};

