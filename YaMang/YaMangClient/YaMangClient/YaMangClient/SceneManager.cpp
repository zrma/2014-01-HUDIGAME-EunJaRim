#include "stdafx.h"
#include "SceneManager.h"
#include "SceneLoad.h"
#include "ScenePlay.h"
#include "SceneResult.h"

SceneManager::SceneManager()
{
	ZeroMemory( &m_SceneList, sizeof( m_SceneList ) );
}


SceneManager::~SceneManager()
{
}

void SceneManager::Init()
{
	// m_SceneList[SCENE_LOAD] = new SceneLoad();

	m_SceneList[SCENE_PLAY] = new ScenePlay();
	m_SceneList[SCENE_PLAY]->Create();

	// m_SceneList[SCENE_RESULT] = new SceneResult();
}

void SceneManager::Destroy()
{
	for ( auto iter : m_SceneList )
	{
		if ( iter != nullptr )
		{
			auto& toBeDelete = iter;
			toBeDelete->Destroy();
			delete toBeDelete;
		}
	}
}

void SceneManager::ChangeScene( SceneType scene )
{
	switch ( scene )
	{
		case SCENE_LOAD:
			if ( m_SceneList[SCENE_LOAD] )
			{
				m_NowScene = m_SceneList[SCENE_LOAD];
			}
			break;
		case SCENE_PLAY:
			if ( m_SceneList[SCENE_PLAY] )
			{
				m_NowScene = m_SceneList[SCENE_PLAY];
			}
			break;
		case SCENE_RESULT:
			if ( m_SceneList[SCENE_RESULT] )
			{
				m_NowScene = m_SceneList[SCENE_RESULT];
			}
			break;
		default:
			break;
	}
}

void SceneManager::Update()
{
	if ( m_NowScene )
	{
		m_NowScene->Update();
	}
}

void SceneManager::Render()
{
	if ( m_NowScene )
	{
		m_NowScene->Render();
	}
}
