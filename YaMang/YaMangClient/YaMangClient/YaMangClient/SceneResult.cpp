#include "stdafx.h"
#include "SceneResult.h"
#include "yaMangDxDll.h"
#include "EnumSet.h"
#include "PlayerManager.h"
#include "TextManager.h"


SceneResult::SceneResult()
{
}


SceneResult::~SceneResult()
{
}

void SceneResult::Create()
{

}

void SceneResult::Destroy()
{

}

void SceneResult::Update()
{

}

void SceneResult::Render() const
{
	RenderSprite(SPRITE_RESULT_BACKGROUND, m_Position);
	TextManager::GetInstance()->DrawTexts();
}
