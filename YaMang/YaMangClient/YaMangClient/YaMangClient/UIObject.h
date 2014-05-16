#pragma once
#include "Interface3D.h"

enum UIType
{
	UI_TYPE_NONE,
	
	UI_TYPE_MAP,

	UI_TYPE_MAX
};

class UIObject :
	public Interface3D
{
public:
	UIObject();
	~UIObject();

	virtual void			Update();
	virtual void			Render() const;

	UIType			GetUIType() { return m_UIType; }
	D3DXVECTOR3		GetUIPos() { return m_UIPos; }
	int				GetUIPosX() { return m_UIPos.x; }
	int				GetUIPosY() { return m_UIPos.y; }
	bool			GetVisible() { return m_Visible; }

	void			SetUIPosition( int x, int y );

private:
	UIType				m_UIType = UI_TYPE_NONE;
	D3DXVECTOR3			m_UIPos;
	bool				m_Visible;
};

