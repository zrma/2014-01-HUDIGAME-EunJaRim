#include "stdafx.h"
#include "ScenePlay.h"
#include "Interface3D.h"
#include "Corps.h"
#include "MacroSet.h"

ScenePlay::ScenePlay()
{
}


ScenePlay::~ScenePlay()
{
}

void ScenePlay::Create()
{

}

void ScenePlay::Destroy()
{
	for ( auto iter : m_CorpsList )
	{
		auto& toBeDelete = iter.second;
		SafeDelete( toBeDelete );
	}
}

void ScenePlay::Update()
{
	for ( auto& iter : m_CorpsList )
	{
		auto& corps = iter.second;
		corps->Update();
	}
}

void ScenePlay::Render()
{
	for ( auto& iter : m_CorpsList )
	{
		auto& corps = iter.second;
		corps->Render();
	}
}

void ScenePlay::AddCorps( int corpsID, Corps* corps )
{
	m_CorpsList[corpsID] = corps;
}
