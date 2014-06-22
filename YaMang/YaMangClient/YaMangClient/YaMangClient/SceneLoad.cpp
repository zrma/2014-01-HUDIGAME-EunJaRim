#include "stdafx.h"
#include "SceneLoad.h"
#include "yaMangDxDll.h"
#include "EnumSet.h"
<<<<<<< HEAD
#include "SoundManager.h"
=======
>>>>>>> 7dcf2c7b0acdc2245fe7241a99e93c22a00241eb

SceneLoad::SceneLoad()
{
}


SceneLoad::~SceneLoad()
{
}

void SceneLoad::Create()
{
<<<<<<< HEAD
	SoundManager::GetInstance()->StopAllSound();
	SoundManager::GetInstance()->PlaySound( SOUND_SCENE_PLAY_BGM3 );
	SoundManager::GetInstance()->SetVolume( SOUND_SCENE_PLAY_BGM3, 0.3f );
=======

>>>>>>> 7dcf2c7b0acdc2245fe7241a99e93c22a00241eb
}

void SceneLoad::Destroy()
{

}

void SceneLoad::Update()
{

}

void SceneLoad::Render() const
{
	RenderSprite( SPRITE_LOADING, m_Position );
}
