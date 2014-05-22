#include "stdafx.h"
#include "CorpsMark.h"
#include "UIObject.h"
#include "UIManager.h"
#include "Corps.h"


CorpsMark::CorpsMark(SpriteKeyType key, SceneType scene, int PosX, int PosY, bool visible, Corps* corps, bool ourCorps)
{
	SetSpriteType(key);
	SetUIPosX(PosX);
	SetUIPosY(PosY);
	SetVisible(visible);
	SetSceneType(scene);
	SetCorps(corps);
	SetIsOurCorps(ourCorps);

	UIManager::GetInstance()->AddUIObject(this);
}


CorpsMark::~CorpsMark()
{
}

void CorpsMark::Update()
{
	UIObject::Update();
	// 부대위치 실시간 확인 변경
	// 부대 죽었는지 실시간으로 확인해서 플래그 변경
}

void CorpsMark::Render() const
{
	UIObject::Render();
}
