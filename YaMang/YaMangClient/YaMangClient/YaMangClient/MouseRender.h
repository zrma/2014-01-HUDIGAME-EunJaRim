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

	HRESULT				CreateCursor( LPCWSTR cursorImagePath, float cursorPosX = 0, float cursorPosY = 0 );
	void				DestroyCursor();

	HRESULT				RenderCursor() const;
	void				SetCursorPos(float PosX, float PosY);

	void				RemoveWndCursor();
	void				SetCursorType(CursorType cursorType) { m_NowCursorType = cursorType; }

private:
	bool				m_IsCursorReady = false;
	CursorType			m_NowCursorType = CURSOR_NONE;
};

