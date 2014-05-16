#pragma once

class UIObject;

enum UIType
{
	UI_TYPE_NONE,

	UI_TYPE_MAP,

	UI_TYPE_MAX
};

class UIManager
{
public:
	UIManager();
	~UIManager();

private:
	std::array<UIObject*, UI_TYPE_MAX> m_UIList;
};

