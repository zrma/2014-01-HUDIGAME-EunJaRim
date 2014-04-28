#pragma once
#include "MacroSet.h"

class MouseRender : public Singleton <MouseRender>
{
public:
	MouseRender();
	~MouseRender();

public:
	HRESULT		CreateCursor( LPCWSTR cursorImagePath, float cursorPosX = 0, float cursorPosY = 0 );
	HRESULT		RenderCursor();
	void		CleanupCursor();
	void		RemoveWndCursor();

private:
	bool		m_IsCursorReady = false;
};

