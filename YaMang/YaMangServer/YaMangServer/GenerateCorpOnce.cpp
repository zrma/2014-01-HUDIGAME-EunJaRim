#include "stdafx.h"
#include "GenerateCorpOnce.h"
#include "GameRoom.h"
#include "SharedDefine.h"
#include "PacketType.h"
#include "Corps.h"
#include "ClientSession.h"
#include "MacroSet.h"

GenerateCorpOnce::GenerateCorpOnce()
{
}


GenerateCorpOnce::~GenerateCorpOnce()
{
}

void GenerateCorpOnce::OnBegin()
{
	Log( "[%d]GenerateCorpOnce OnBegin \n", m_PlayerID );

	m_ActionStatus = ACTION_TICK;
	m_ClientManager->AddActionToScheduler( this, 0 );
}

void GenerateCorpOnce::OnTick()
{
	if ( m_PlayerID == -1 )
	{
		m_ActionStatus = ACTION_END;
		m_ClientManager->AddActionToScheduler( this, 0 );
		return;
	}

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

	// 하드 코딩중... 랜덤으로 나와라1
	float nowX = static_cast<float>( rand() % 30 );
	float nowZ = static_cast<float>( rand() % 30 );
	float lookX = 0.0f;
	float lookZ = 0.0f;

	PositionInfo position;
	position.m_EyePoint = { nowX, 0.0f, nowZ };
	position.m_LookAtPoint = { lookX, 0.0f, lookZ };

	if ( nullptr == m_Corps ) ///# if ( m_Corps)
	{
		m_Corps = m_ClientManager->GenerateCorps( m_PlayerID, unitType, position );
	}

	position = m_Corps->GetPositionInfo();
	GenerateCorpsResult outPacket;
	outPacket.m_UnitType = m_Corps->GetUnitType();
	outPacket.m_NowX = position.m_EyePoint.x;
	outPacket.m_NowZ = position.m_EyePoint.z;
	outPacket.m_LookX = position.m_LookAtPoint.x;
	outPacket.m_LookZ = position.m_LookAtPoint.z;
	outPacket.m_CorpsID = m_Corps->GetCorpsID();
	outPacket.m_PlayerId = m_PlayerID;

	outPacket.m_FormationType = m_Corps->GetFormationType( );

	if ( m_CorpsHP > 0)
	{
		const_cast<Corps*>( m_Corps )->SetHP( m_CorpsHP ); // 우와 const_cast... 설계가 정말 꼬여간다!!!!! 
		///#  const_cast -.-; 왜 이렇게...  정 안되면 mutable도 고려 가능..
	}
	outPacket.m_UnitNum = m_Corps->GetUnitNum( );


	m_ClientManager->BroadcastPacket( &outPacket );

	Log( "GenerateCorpOnce! Type:%d CorpID:%d PlayerID:%d \n",
			  unitType, m_Corps->GetCorpsID( ), m_PlayerID );

	m_ClientSession->AddCorpsNum();
	m_ActionStatus = ACTION_END;
	m_ClientManager->AddActionToScheduler( this, 0 );
}

void GenerateCorpOnce::OnEnd()
{
	Log( "GenerateCorpOnce OnEnd \n" );

	Action::OnEnd();
}
