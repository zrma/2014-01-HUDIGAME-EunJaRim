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
	void				Create();
	void				Delete();
	virtual void		Update();
	virtual void		Render();

	HRESULT				CreateCursor( LPCWSTR cursorImagePath, float cursorPosX = 0, float cursorPosY = 0 );
	HRESULT				RenderCursor();
	void				CleanupCursor();
	void				SetCursorPos(float PosX, float PosY);

	void				RemoveWndCursor();

	void				SetSceneType(CursorType cursorType) { m_NowCursorType = cursorType; }

private:
	bool				m_IsCursorReady = false;
	CursorType			m_NowCursorType = CURSOR_NONE;
};

