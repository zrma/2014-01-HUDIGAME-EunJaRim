#pragma once
#include "MacroSet.h"
#include "Interface3D.h"
#include "EnumSet.h"

enum CursorTextureType
{
	CURSOR_TEXTURE_NONE,
	
	CURSOR_TEXTURE_BASIC,
	CURSOR_TEXTURE_CLICK,
	CURSOR_TEXTURE_OVER_CORPS,
	CURSOR_TEXTURE_CLICK_CORPS,
	CURSOR_TEXTURE_ATTACK,
	CURSOR_TEXTURE_UNRECHEABLE,
	
	CURSOR_TEXTURE_MAX
};

class MouseRender : public Interface3D
{
public:
	MouseRender();
	~MouseRender();

public:
	void				Init();
	virtual void		Update();
	virtual void		Render() const;
	
	void				SetGameCursorPos( int PosX, int PosY );

	void				SetCursorRenderType(CursorTextureType renderType) { m_CursorRenderType = renderType; }
	CursorTextureType	GetCursorRenderType() { return m_CursorRenderType; }
	void				ChangeCursorRenderType();

private:
	bool				m_IsCursorReady = false;
	CursorTextureType	m_CursorRenderType = CURSOR_TEXTURE_NONE;
};

