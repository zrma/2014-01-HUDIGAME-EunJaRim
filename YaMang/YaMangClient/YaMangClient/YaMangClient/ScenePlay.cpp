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
	//////////////////////////////////////////////////////////////////////////
	// 임시로 이렇게 해 두었지만
	// 나중에 내부적으로 Corps를 추가하거나 제거하는 코드 작성해야 함
	/*
	Corps* corps = new Corps();
	corps->Create( 1, UNIT_ARROW );
	corps->SetVisible( true );
	m_CorpsList[100] = corps;

	corps = new Corps();
	corps->Create( 1, UNIT_GUARD );
	corps->SetVisible( true );
	m_CorpsList[200] = corps;

	corps = new Corps();
	corps->Create( 1, UNIT_KNIGHT );
	corps->SetVisible( true );
	m_CorpsList[300] = corps;

	corps = new Corps();
	corps->Create( 1, UNIT_PIKE );
	corps->SetVisible( true );
	m_CorpsList[400] = corps;

	corps = new Corps();
	corps->Create( 1, UNIT_SWORD );
	corps->SetVisible( true );
	m_CorpsList[500] = corps;
	*/
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
