#include "stdafx.h"
#include "GenerateCorpAction.h"
#include "EnumSet.h"
#include "SharedDefine.h"
#include "ClientManager.h"
#include "PacketType.h"
#include "ClientSession.h"
#include "Corps.h"

GenerateCorpAction::GenerateCorpAction( )
{
}


GenerateCorpAction::~GenerateCorpAction( )
{
}

void GenerateCorpAction::OnBegin( )
{
	printf_s( "[%d]GenerateCorps OnBegin \n", m_PlayerID);
	m_ActionStatus = ACTION_TICK;
	m_ClientManager->AddActionToScheduler( this, 0 );
}

void GenerateCorpAction::OnTick( )
{
	if ( m_PlayerID == -1 )
	{
		m_ActionStatus = ACTION_END;
		m_ClientManager->AddActionToScheduler( this, 0 );
	}

	UnitType unitType = UnitType::UNIT_NONE;

	int unitRand = rand( ) % 5;

	switch ( unitRand )
	{
		case 0:
			unitType = UnitType::UNIT_ARROW;
			break;
		case 1:
			unitType = UnitType::UNIT_GUARD;
			break;
		case 2:
			unitType = UnitType::UNIT_KNIGHT;
			break;
		case 3:
			unitType = UnitType::UNIT_PIKE;
			break;
		case 4:
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

	const Corps* corps = m_ClientManager->GenerateCorps( m_PlayerID, unitType, position );


	GenerateCorpsResult outPacket;
	outPacket.m_UnitType = unitType;
	outPacket.m_NowX = nowX;
	outPacket.m_NowZ = nowZ;
	outPacket.m_LookX = lookX;
	outPacket.m_LookZ = lookZ;
	outPacket.m_CorpsID = corps->GetCorpsID();
	outPacket.m_PlayerId = m_PlayerID;

	outPacket.m_FormationType = corps->GetFormationType();
	outPacket.m_UnitNum = corps->GetUnitNum();


	m_ClientManager->BroadcastPacket( &outPacket );

	printf_s( "GenerateCorps! Type:%d CorpID:%d PlayerID:%d \n",
			  unitType, corps->GetCorpsID( ), m_PlayerID );

	m_ClientSession->AddCorpsNum();

	m_ActionStatus = ACTION_TICK;
	m_ClientManager->AddActionToScheduler( this, m_ClientSession->GetCorpsRegenTime() );
}

void GenerateCorpAction::OnEnd( )
{
	printf_s( "GenerateCorps OnEnd \n" );

	Action::OnEnd();
}
