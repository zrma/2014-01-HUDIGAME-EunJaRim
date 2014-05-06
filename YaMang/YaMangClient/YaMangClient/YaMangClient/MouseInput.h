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

	MousePos		GetMousePosition() const { return m_MousePosition; }
	int				GetMousePositionX() const { return m_XPos; }
	int				GetMousePositionY() const { return m_YPos; }
	void			SetMousePosition( int xPos, int yPos ) { m_XPos = xPos; m_YPos = yPos; }

	MouseStatus		GetMouseStatus() const { return m_MouseStatus; }
	void			SetMouseStatus(MouseStatus mouseStatus) { m_MouseStatus = mouseStatus; }

	void			MoveMousePosition(int x, int y) { m_XPos += x; m_YPos += y; }

private:
	//윈도우 커서 좌표 저장
	int				m_WndXPos = 0;
	int				m_WndYPos = 0;
	MousePos		m_WndMousePos;

	//현재 마우스 좌표 저장
	int				m_XPos = 200;
	int				m_YPos = 200;
	MousePos		m_MousePosition;

	//드래그 시작 마우스 좌표 저장
	int				m_PressedXPos = 200;
	int				m_PressedYPos = 200; 
	MousePos		m_PressedMousePosition;

	MouseStatus		m_MouseStatus = MouseStatus::MOUSE_NONE;
};

