#pragma once

#include <d3dx9math.h>

class UIObject //: public Interface3D
{
public:
	UIObject();
	~UIObject();

	virtual void		Update();
	virtual void		Render() const;

	D3DXVECTOR3		GetUIPos() { return m_UIPosition; }
	float			GetUIPosX() { return m_UIPosition.x; }
	float			GetUIPosY() { return m_UIPosition.y; }
	bool			GetVisible() { return m_Visible; }
	void			SetUIPosition(int x, int y);

	void			SetUIPosX(int x) { m_UIPosition.x = static_cast<float>(x); }
	void			SetUIPosY(int y) { m_UIPosition.y = static_cast<float>(y); }
	void			SetVisible(bool visible) { m_Visible = visible; }

private:
	D3DXVECTOR3			m_UIPosition = {0, 0, 0};
	bool				m_Visible = true;
};

