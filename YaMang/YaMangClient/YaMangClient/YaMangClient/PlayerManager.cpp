#include "stdafx.h"
#include "PlayerManager.h"
#include "NetworkManager.h"


PlayerManager::PlayerManager()
{
}


PlayerManager::~PlayerManager()
{
	ClearSelectedCorps();
}

bool PlayerManager::IsCorpsInIdList( int corpsId ) const
{
	if ( !m_SelectedCorpsList.empty() )
	{
		for ( auto& iter : m_SelectedCorpsList )
		{
			if ( iter == corpsId )
			{
				return true;
			}
		}
	}

	return false;
}

void PlayerManager::AttackCorpsById( int corpsID ) const
{
	if ( !m_SelectedCorpsList.empty() )
	{
		for ( auto& iter : m_SelectedCorpsList )
		{
			AttackCorpsRequest attackCorpsData;
			attackCorpsData.m_MyCorpsID = iter;
			attackCorpsData.m_TargetCorpsID = corpsID;

			NetworkManager::GetInstance()->SendPacket( &attackCorpsData );
		}
	}
}

void PlayerManager::MoveCorpsToPosition( float x, float z ) const
{
	if ( !m_SelectedCorpsList.empty() )
	{
		for ( auto& iter : m_SelectedCorpsList )
		{
			MoveCorpsRequest moveCorpsData;
			moveCorpsData.m_CorpsID = iter;
			moveCorpsData.m_NowX = x;
			moveCorpsData.m_NowZ = z;
			moveCorpsData.m_TargetX = x;
			moveCorpsData.m_TargetZ = z;

			NetworkManager::GetInstance()->SendPacket( &moveCorpsData );
		}
	}
}

void PlayerManager::ChangeCorpsFormation( FormationType formation ) const
{
	if ( !m_SelectedCorpsList.empty() )
	{
		for ( auto& iter : m_SelectedCorpsList )
		{
			ChangeCorpsFormationRequest changeCorpsFormationData;
			changeCorpsFormationData.m_CorpsID = iter;
			changeCorpsFormationData.m_FormationType = formation;

			NetworkManager::GetInstance()->SendPacket( &changeCorpsFormationData );
		}
	}
}

