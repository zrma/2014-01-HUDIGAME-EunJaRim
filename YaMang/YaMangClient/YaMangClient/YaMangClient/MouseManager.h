#pragma once
#include "MacroSet.h"

enum class MouseStatus
{
	MOUSE_NONE,
	MOUSE_DOWN,
	MOUSE_PRESSED,
	MOUSE_UP
};

class MouseManager: public Singleton<MouseManager>
{
public:
	MouseManager();
	~MouseManager();

	COORD			GetMousePosition() const { return m_MousePosition; }
	int				GetMousePositionX() const { return m_MousePosition.X; }
	int				GetMousePositionY() const { return m_MousePosition.Y; }
	void			SetMousePosition( int posX, int posY ) { m_MousePosition.X = posX; m_MousePosition.Y = posY; }

	MouseStatus		GetMouseStatus() const { return m_MouseStatus; }
	void			SetMouseStatus( MouseStatus mouseStatus ) { m_MouseStatus = mouseStatus; }

	void			MoveMousePosition( int posX, int posY );

	bool			IsGameCursorMode() { return m_GameCursorMod; }
	void			SetGameCursorMode( bool gameCursorMode ) { m_GameCursorMod = gameCursorMode; }

	void			SetWndLocation( int posX, int posY ) { m_WndLocationX = posX; m_WndLocationY = posY; }

private:
	//윈도우 커서 고정 좌표 저장
	int				m_WndXPos = 0;
	int				m_WndYPos = 0;

	//윈도우 위치 저장
	int				m_WndLocationX = 0;
	int				m_WndLocationY = 0;

	// 바운더리
	COORD			m_Boundary;

	//현재 마우스 좌표 저장
	COORD			m_MousePosition;

	//드래그 시작 마우스 좌표 저장
	COORD			m_PressedMousePosition;

	bool			m_GameCursorMod = false;
	MouseStatus		m_MouseStatus = MouseStatus::MOUSE_NONE;
};
