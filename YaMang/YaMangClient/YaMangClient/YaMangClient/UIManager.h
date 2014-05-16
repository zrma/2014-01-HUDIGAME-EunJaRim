#pragma once
#include "MacroSet.h"

class UIObject;

enum UIType
{
	UI_TYPE_NONE,

	UI_TYPE_MAP,

	UI_TYPE_MAX
};

class UIManager :public Singleton<UIManager>
{
public:
	UIManager();
	~UIManager();

	void		Update();
	void		Render() const;

	HRESULT		InitUI();
	void		RegistUI( UIType key, int PosX, int PosY, bool visible);
	void		CleanUp();

private:
	std::array<UIObject*, UI_TYPE_MAX> m_UIList;

	bool		m_IsUIReady = false;
};

