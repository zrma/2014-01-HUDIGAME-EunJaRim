#include "stdafx.h"
#include "SharedDefine.h"
#include "Corps.h"
#include "ScenePlay.h"
#include "MacroSet.h"

#include "EnumSet.h"
#include "Action.h"
#include "SoundManager.h"
#include "MouseRender.h"

ScenePlay::ScenePlay()
{
}


ScenePlay::~ScenePlay()
{
}

//렌더 매니저 초기화가 우선되어야 함
void ScenePlay::Create()
{
	SoundManager::GetInstance()->PlaySound( SOUND_SCENE_PLAY_BGM1 );

	m_MouseCursor = new MouseRender();
	//////////////////////////////////////////////////////////////////////////
	// 임시로 이렇게 해 두었지만
	// 나중에 내부적으로 Corps를 추가하거나 제거하는 코드 작성해야 함
	
	/*PositionInfo pos;
	pos.m_EyePoint = { 0.0f, 0.0f, 10.0f };
	pos.m_LookAtPoint = { 0.0f, 0.0f, 9.0f };
	Corps* corps = new Corps( 1, 1, pos );
	corps->Create( 6, UnitType::UNIT_ARROW );
	corps->SetVisible( true );
	m_CorpsList[100] = corps;

	pos.m_EyePoint = { -15.0f, 0.0f, 10.0f };
	pos.m_LookAtPoint = { -14.0f, 0.0f, 9.0f };
	corps = new Corps( 2, 1, pos );
	corps->SetFormation( FormationType::FORMATION_DESTROY );
	corps->Create( 10, UnitType::UNIT_GUARD );
	corps->SetVisible( true );
	m_CorpsList[200] = corps;

	pos.m_EyePoint = { 15.0f, 0.0f, 10.0f };
	pos.m_LookAtPoint = { 14.0f, 0.0f, 9.0f };
	corps = new Corps( 3, 1, pos );
	corps->Create( 8, UnitType::UNIT_PIKE );
	corps->SetVisible( true );
	m_CorpsList[300] = corps;

	pos.m_EyePoint = { 8.0f, 0.0f, 10.0f };
	pos.m_LookAtPoint = { 7.0f, 0.0f, 9.0f };
	corps = new Corps( 4, 1, pos );
	corps->SetFormation( FormationType::FORMATION_RUSH );
	corps->Create( 6, UnitType::UNIT_KNIGHT );
	corps->SetVisible( true );
	m_CorpsList[400] = corps;

	pos.m_EyePoint = { -8.0f, 0.0f, 10.0f };
	pos.m_LookAtPoint = { -7.0f, 0.0f, 9.0f };
	corps = new Corps( 5, 1, pos );
	corps->SetFormation( FormationType::FORMATION_DEFENSE );
	corps->Create( 8, UnitType::UNIT_SWORD );
	corps->SetVisible( true );
	m_CorpsList[500] = corps;*/
}

void ScenePlay::Destroy()
{
	SoundManager::Release();

	for ( auto iter : m_CorpsList )
	{
		auto& toBeDelete = iter.second;
		SafeDelete( toBeDelete );
	}
	m_CorpsList.clear();

	SafeDelete( m_MouseCursor );
}

void ScenePlay::Update()
{
	for ( auto& iter : m_CorpsList )
	{
		auto& corps = iter.second;
		corps->Update();
	}

	m_MouseCursor->Update();
}

void ScenePlay::Render() const
{
	for ( auto& iter : m_CorpsList )
	{
		auto& corps = iter.second;
		corps->Render();
	}

	m_MouseCursor->Render();
}

void ScenePlay::AddCorps( int corpsID, Corps* corps )
{
	if ( m_CorpsList.find( corpsID ) == m_CorpsList.end() )
	{
		m_CorpsList[corpsID] = corps;
	}
	else
	{
		SafeDelete( corps );

		// 중복 ID로 다시 보냈음
		assert( false );
	}
}

void ScenePlay::ChangeCorpsFormation( int corpsID, FormationType formationType )
{
	if ( m_CorpsList.find( corpsID ) != m_CorpsList.end() )
	{
		if ( m_CorpsList[corpsID] == nullptr )
		{
			assert( false );
		}
		m_CorpsList[corpsID]->SetFormation( formationType );
		ActionTransFormation action;
		m_CorpsList[corpsID]->ChangeAction( action );
	}
}

void ScenePlay::MoveCorpsStart( int corpsID, D3DXVECTOR3 targetPosition, D3DXVECTOR3 lookAtVector, float speed )
{
	if ( m_CorpsList.find( corpsID ) != m_CorpsList.end() )
	{
		if ( m_CorpsList[corpsID] == nullptr )
		{
			assert( false );
		}
		m_CorpsList[corpsID]->SetTargetPosition( targetPosition );
		m_CorpsList[corpsID]->SetLookAtVector( lookAtVector );
		m_CorpsList[corpsID]->SetSpeed( speed );

		ActionMovePosition action;
		m_CorpsList[corpsID]->ChangeAction( action );
	}
}

void ScenePlay::MoveCorpsStop( int corpsID, D3DXVECTOR3 targetPosition, D3DXVECTOR3 lookAtVector )
{
	if ( m_CorpsList.find( corpsID ) != m_CorpsList.end() )
	{
		if ( m_CorpsList[corpsID] == nullptr )
		{
			assert( false );
		}
		m_CorpsList[corpsID]->SetTargetPosition( targetPosition );
		m_CorpsList[corpsID]->SetLookAtVector( lookAtVector );
		m_CorpsList[corpsID]->SetSpeed( 0 );
		m_CorpsList[corpsID]->ClearAction();
	}
}

void ScenePlay::SetCorpsHP( int corpsID, int unitNum )
{
	if ( m_CorpsList.find( corpsID ) != m_CorpsList.end() )
	{
		if ( m_CorpsList[corpsID] == nullptr )
		{
			assert( false );
		}
		m_CorpsList[corpsID]->SetCorpsHP( unitNum );
	}
}

UnitType ScenePlay::GetUnitTypeByID( int corpsID )
{
	if ( m_CorpsList.find( corpsID ) != m_CorpsList.end() )
	{
		if ( m_CorpsList[corpsID] == nullptr )
		{
			assert( false );
		}
		return m_CorpsList[corpsID]->GetUnitType();
	}
	return UnitType::UNIT_NONE;
}

