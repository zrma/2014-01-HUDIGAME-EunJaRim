#pragma once
#include "MacroSet.h"
#include "Interface3D.h"

class MouseRender : public Interface3D
{
public:
	MouseRender();
	~MouseRender();

public:
	virtual void		Update();
	virtual void		Render();

	HRESULT				CreateCursor( LPCWSTR cursorImagePath, float cursorPosX = 0, float cursorPosY = 0 );
	HRESULT				RenderCursor();
	void				CleanupCursor();
	void				RemoveWndCursor();

private:
	bool		m_IsCursorReady = false;
};

