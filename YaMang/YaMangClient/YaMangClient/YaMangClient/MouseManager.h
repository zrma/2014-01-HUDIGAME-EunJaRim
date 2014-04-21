#pragma once
#include "MacroSet.h"

class MouseManager : public Singleton <MouseManager>
{
public:
	MouseManager();
	~MouseManager();

public:
	HRESULT CreateCursor( LPCWSTR cursorImagePath, float cursorPosX = 0, float cursorPosY = 0 );
	HRESULT RenderCursor();
	void CleanupCursor();
	void SetCursorPos( float PosX, float PosY );

private:
	bool m_IsCursorReady = false;

};

