#pragma once
#include "Interface3D.h"

class UIObject :
	public Interface3D
{
public:
	UIObject();
	~UIObject();

	virtual void			Update();
	virtual void			Render() const;

	UIType			GetUIType() { return m_UIType; }
	D3DXVECTOR3		GetUIPos() { return m_UIPosition; }
	int				GetUIPosX() { return m_UIPosition.x; }
	int				GetUIPosY() { return m_UIPosition.y; }
	bool			GetVisible() { return m_Visible; }

private:
	D3DXVECTOR3			m_UIPosition;
	bool				m_Visible;
};

