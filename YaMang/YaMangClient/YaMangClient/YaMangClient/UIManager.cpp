#include "stdafx.h"
#include "UIManager.h"
#include "UIObject.h"
#include "YaMangDxDll.h"
#include "ResourceDef.h"


UIManager::UIManager()
{
}


UIManager::~UIManager()
{
}

void UIManager::Init()
{

}

void UIManager::Update()
{
	for (auto &iter : m_UIList)
	{
		iter->Update();
	}
}

void UIManager::Render() const
{
	for (auto &iter : m_UIList)
	{
		iter->Render();
	}
}

void UIManager::AddUIObject( UISpriteKeyType key, int PosX, int PosY, bool visible)
{
	UIObject* Object = new UIObject();
	Object->SetUIPosX(PosX);
	Object->SetUIPosY(PosY);
	Object->SetVisible(visible);

	m_UIList.push_back(Object);
}

void UIManager::Destroy()
{

}

