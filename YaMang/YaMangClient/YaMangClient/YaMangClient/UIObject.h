#pragma once

#include <d3dx9math.h>
#include "EnumSet.h"

class UIObject //: public Interface3D
{
public:
	UIObject();
	~UIObject();

	virtual void		Update();
	virtual void		Render() const;

	D3DXVECTOR3			GetUIPos() { return m_UIPosition; }
	float				GetUIPosX() { return m_UIPosition.x; }
	float				GetUIPosY() { return m_UIPosition.y; }
	bool				GetVisible() { return m_Visible; }
	bool				GetSuicide() { return m_Suicide; }

	SceneType			GetSceneType() { return m_SceneType; }
	UISpriteKeyType		GetSpriteType() { return m_UISpriteType; }

	void				SetUIPosition(int x, int y);
	void				SetUIPosX(int x) { m_UIPosition.x = static_cast<float>(x); }
	void				SetUIPosX(float x) { m_UIPosition.x = x; }
	void				SetUIPosY(int y) { m_UIPosition.y = static_cast<float>(y); }
	void				SetUIPosY(float y) { m_UIPosition.y = y; }
	void				SetVisible(bool visible) { m_Visible = visible; }
	void				SetSuicide(bool suicide) { m_Suicide = suicide; }

	void				SetSceneType(SceneType type) { m_SceneType = type; }
	void				SetSpriteType(UISpriteKeyType type) { m_UISpriteType = type; }

private:
	D3DXVECTOR3			m_UIPosition = {0, 0, 0};
	bool				m_Visible = true;
	bool				m_Suicide = false;

	UISpriteKeyType		m_UISpriteType = UISPRITE_NONE;
	SceneType			m_SceneType = SCENE_NONE;
};

