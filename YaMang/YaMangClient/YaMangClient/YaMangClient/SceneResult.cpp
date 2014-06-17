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
	if ( PlayerManager::GetInstance()->GetWin() )
	{
		RenderSprite( SPRITE_RESULT_WIN, m_Position );
	}
	else
	{
		RenderSprite( SPRITE_RESULT_LOSE, m_Position );
	}

	TextManager::GetInstance()->DrawTexts();
}
