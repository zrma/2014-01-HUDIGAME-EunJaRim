#include "stdafx.h"
#include "SharedDefine.h"
#include "Corps.h"
#include "ScenePlay.h"
#include "MacroSet.h"

#include "EnumSet.h"
#include "Action.h"
#include "SoundManager.h"
#include "MouseRender.h"
#include "NetworkManager.h"
#include "CameraController.h"

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
		// CheckCorps로 이미 체크하므로 필요 없을 듯... 지울까요? 혹시 모르니...
		SafeDelete( corps );

		// 중복 ID로 다시 보냈음
		assert( false );
	}
}

bool ScenePlay::CheckCorps( int corpsID )
{
	if ( m_CorpsList.find( corpsID ) == m_CorpsList.end() )
	{
		return true;
	}
	return false;
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

const Corps* ScenePlay::GetCorpsByID( int corpsID )
{
	if ( m_CorpsList.find( corpsID ) != m_CorpsList.end() )
	{
		return m_CorpsList[corpsID];
	}
	return nullptr;
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

Corps* ScenePlay::SearchCorpsByPosition( float x, float z, bool selectOption )
{
	Corps* resultCorps = nullptr;
	for ( auto& iter : m_CorpsList )
	{
		auto& thisCorps = iter.second;

		// 하나만 피킹 하기 위해서
		if ( resultCorps )
		{
			if ( selectOption )
			{
				thisCorps->SetSelected( false );
			}
			continue;
		}

		if ( thisCorps->GetOwnPlayerID() != NetworkManager::GetInstance()->GetMyPlayerID() )
		{
			// continue;
		}

		D3DXVECTOR3& thisPosition = thisCorps->GetEyePoint();

		double	margin = CameraController::GetInstance()->GetHeightGrade() + 7.0;
		// AABB
		if ( ( thisPosition.x - x < margin && thisPosition.x - x > -margin )
			 && ( thisPosition.x - x < margin && thisPosition.x - x > -margin ) )
		{
			Log( "%d번 부대 1차 피킹!!! \n", thisCorps->GetCorpsID() );
		
			// BoundingCircle
			if ( thisCorps->IsContain( x, z ) )
			{
				Log( "%d번 부대 2차 피킹!!! 성공! \n", thisCorps->GetCorpsID() );
				if ( selectOption )
				{
					thisCorps->SetSelected( true );
				}
				resultCorps = thisCorps;
			}
			else if ( selectOption )
			{
				thisCorps->SetSelected( false );
			}
		}
		else if ( selectOption )
		{
			thisCorps->SetSelected( false );
		}
	}

	return resultCorps;
}
