#pragma once
#include "MacroSet.h"
#include "Interface3D.h"
#include "EnumSet.h"

enum class CursorRenderType
{
	CURSOR_RENDER_NONE,
	CURSOR_RENDER_BASIC,
	CURSOR_RENDER_CLICK,
	CURSOR_RENDER_ON_CORPS,
	CURSOR_RENDER_CLICK_CORPS,
	CURSOR_RENDER_ATTACK,
	CURSOR_RENDER_UNMOVABLE_AREA,
	CURSOR_RENDER_MAX
};

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

	void				SetGameCursorPos( int PosX, int PosY );

	void				SetCursorRenderType(CursorRenderType renderType) { m_CursorRenderType = renderType; }
	CursorRenderType	GetCursorRenderType() { return m_CursorRenderType; }
	void				ChangeCursorRenderType();

private:
	bool				m_IsCursorReady = false;
	CursorRenderType	m_CursorRenderType = CursorRenderType::CURSOR_RENDER_NONE;
};

