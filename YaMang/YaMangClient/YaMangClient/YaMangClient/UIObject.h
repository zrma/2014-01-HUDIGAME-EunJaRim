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

	D3DXVECTOR3		GetUIPos() { return m_UIPosition; }
	float			GetUIPosX() { return m_UIPosition.x; }
	float			GetUIPosY() { return m_UIPosition.y; }
	bool			GetVisible() { return m_Visible; }
	void			SetUIPosition(int m_UIType, int x, int y);
private:
	D3DXVECTOR3			m_UIPosition;
	bool				m_Visible;
};

