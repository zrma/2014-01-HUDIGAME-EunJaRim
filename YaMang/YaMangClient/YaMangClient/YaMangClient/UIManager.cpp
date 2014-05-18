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

void UIManager::Render() const
{
	if (m_IsUIReady)
	{
		//Render
	}
}

HRESULT UIManager::InitUI()
{
	if (FAILED(InitUISprite(UI_TYPE_MAX)))
	{
		return E_FAIL;
	};
	//CreateImage 하고 List에 Regist해야하는 부분

	m_IsUIReady = true;
	return S_OK;
}

void UIManager::RegistUI( UIType key, int PosX, int PosY, bool visible )
{
	UIObject* Object = m_UIList[key];
	Object->SetUIPosX(PosX);
	Object->SetUIPosY(PosY);
	Object->SetVisible(visible);
}

void UIManager::CleanUp()
{
	if (m_IsUIReady)
	{
		UICleanUp();
	}
}

