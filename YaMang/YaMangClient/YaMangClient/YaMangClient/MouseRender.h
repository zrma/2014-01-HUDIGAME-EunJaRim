#pragma once
#include "MacroSet.h"
#include "Interface3D.h"
#include "EnumSet.h"

class MouseRender : public Interface3D
{
public:
	MouseRender();
	~MouseRender();

public:
	void				Init();
	void				Destroy();
	virtual void		Update();
	virtual void		Render() const;

	HRESULT				CreateCursor( LPCWSTR cursorImagePath, int cursorPosX = 500, int cursorPosY = 500 );
	void				DestroyCursor();

	void				SetGameCursorPos(int PosX, int PosY);
	void				SetCursorType(CursorType cursorType) { m_NowCursorType = cursorType; }

private:
	bool				m_IsCursorReady = false;
	CursorType			m_NowCursorType = CURSOR_NONE;
};

