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

	void			MoveMousePosition(int x, int y);

	bool			IsGameCursorMode() { return m_GameCursorMod; }
	void			SetGameCursorMode(bool gameCursorMode) { m_GameCursorMod = gameCursorMode; }

	void			SetWndLocation(int x, int y){ m_WndLocationX = x; m_WndLocationY = y; }

private:
	//윈도우 커서 고정 좌표 저장
	const int		m_WndXPos = 500;
	const int		m_WndYPos = 500;

	//윈도우 위치 저장
	int				m_WndLocationX = 0;
	int				m_WndLocationY = 0;

	//현재 마우스 좌표 저장
	int		m_XPos = 500;
	int		m_YPos = 500;
	MousePos		m_MousePosition;

	//드래그 시작 마우스 좌표 저장
	int				m_PressedXPos = 200;
	int				m_PressedYPos = 200; 
	MousePos		m_PressedMousePosition;

	bool			m_GameCursorMod = false;

	MouseStatus		m_MouseStatus = MouseStatus::MOUSE_NONE;
};

