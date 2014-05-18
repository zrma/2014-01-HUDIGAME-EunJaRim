#include "stdafx.h"
#include "GenerateCorpAction.h"
#include "EnumSet.h"
#include "SharedDefine.h"
#include "GameRoom.h"
#include "PacketType.h"
#include "ClientSession.h"
#include "Corps.h"
#include "MacroSet.h"

GenerateCorpAction::GenerateCorpAction( )
{
}


GenerateCorpAction::~GenerateCorpAction( )
{
}

void GenerateCorpAction::OnBegin( )
{
	Log( "[%d]GenerateCorps OnBegin \n", m_PlayerID);

	m_LastGeneratedTime = GetTickCount64();

	m_ActionStatus = ACTION_TICK;
	m_ClientManager->AddActionToScheduler( this, 0 );
}

void GenerateCorpAction::OnTick()
{
	if ( m_PlayerID == -1 )
	{
		m_ActionStatus = ACTION_END;
		m_ClientManager->AddActionToScheduler( this, 0 );
	}

	if ( GetTickCount64() - m_LastGeneratedTime > m_ClientSession->GetCorpsRegenTime() )
	{
		UnitType unitType = UnitType::UNIT_NONE;

		int unitRand = rand() % 4;

		switch ( unitRand )
		{
			case 0:
				unitType = UnitType::UNIT_ARROW;
				break;
			case 1:
				unitType = UnitType::UNIT_KNIGHT;
				break;
			case 2:
				unitType = UnitType::UNIT_PIKE;
				break;
			case 3:
				unitType = UnitType::UNIT_SWORD;
				break;
			default:
				unitType = UnitType::UNIT_ARROW;
				break;
		}

		
		const PositionInfo& kingPosition = m_ClientManager->GetKingPositionInfo( m_ClientSession->GetKingIndex() );

		float nowX = kingPosition.m_EyePoint.x;
		float nowZ = kingPosition.m_EyePoint.z;

		// 하드 코딩중... 왕 앞쪽으로 랜덤으로 나와라2
		int spawnRange = 20;
		int xDirection = 1;
		int zDirection = 1;

		if ( nowX > 0 )
		{
			xDirection = -1;
		}

		if ( nowZ > 0 )
		{
			zDirection = -1;
		}

		nowX += xDirection * ( spawnRange / 2 );
		nowZ += zDirection * ( spawnRange / 2 );

		nowX += static_cast<float>( xDirection * ( rand() % spawnRange ) );
		nowZ += static_cast<float>( zDirection * ( rand( ) % spawnRange ) );

		float lookX = 0.0f;
		float lookZ = 0.0f;

		PositionInfo position;
		position.m_EyePoint = { nowX, 0.0f, nowZ };
		position.m_LookAtPoint = { lookX, 0.0f, lookZ };

		const Corps* corps = m_ClientManager->GenerateCorps( m_PlayerID, unitType, position );


		GenerateCorpsResult outPacket;
		outPacket.m_UnitType = corps->GetUnitType();
		outPacket.m_NowX = position.m_EyePoint.x;
		outPacket.m_NowZ = position.m_EyePoint.z;
		outPacket.m_LookX = position.m_LookAtPoint.x;
		outPacket.m_LookZ = position.m_LookAtPoint.z;
		outPacket.m_CorpsID = corps->GetCorpsID();
		outPacket.m_PlayerId = m_PlayerID;

		outPacket.m_FormationType = corps->GetFormationType();
		outPacket.m_UnitNum = corps->GetUnitNum();


		m_ClientManager->BroadcastPacket( &outPacket );

		Log( "GenerateCorps! Type:%d CorpID:%d PlayerID:%d \n",
			 unitType, corps->GetCorpsID(), m_PlayerID );

		m_LastGeneratedTime = GetTickCount64();
	}


	// 0.5초마다 리젠타임이 되었는지 확인
	m_ActionStatus = ACTION_TICK;
	m_ClientManager->AddActionToScheduler( this, 500 );
}

void GenerateCorpAction::OnEnd( )
{
	Log( "GenerateCorps OnEnd \n" );

	Action::OnEnd();
}
