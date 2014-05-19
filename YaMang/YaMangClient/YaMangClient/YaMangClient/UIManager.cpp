#include "stdafx.h"
#include "UIManager.h"
#include "UIObject.h"
#include "YaMangDxDll.h"
#include "ResourceDef.h"
#include "SceneManager.h"


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
	m_NowSceneType = SceneManager::GetInstance()->GetNowSceneType();

	for (auto &iter : m_UIList)
	{
		iter->Update();

		if (iter->GetSuicide() == true)
		{
			UIObject* tobeRemoved = iter;
			SafeDelete(tobeRemoved);
			m_UIList.remove(tobeRemoved);
		}
	}
}

void UIManager::Render() const
{
	for (auto &iter : m_UIList)
	{
		if (iter->GetVisible() == true && (iter->GetSceneType() == m_NowSceneType))
		{
			iter->Render();
		}
	}
}

void UIManager::AddUIObject(UIObject* Object)
{
	m_UIList.push_back(Object);
}

void UIManager::Destroy()
{

}

