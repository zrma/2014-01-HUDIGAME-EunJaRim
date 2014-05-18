#include "stdafx.h"
#include "SceneLoad.h"
#include "yaMangDxDll.h"
#include "EnumSet.h"
#include "MainWindow.h"

SceneLoad::SceneLoad()
{
}


SceneLoad::~SceneLoad()
{
}

void SceneLoad::Create()
{

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
