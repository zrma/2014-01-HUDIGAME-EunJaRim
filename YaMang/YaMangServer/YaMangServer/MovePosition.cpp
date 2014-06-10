#include "stdafx.h"
#include "MovePosition.h"
#include "ActionScheduler.h"
#include "PacketType.h"
#include "GameRoom.h"
#include "Corps.h"
#include "MacroSet.h"


MovePosition::MovePosition()
{
}

MovePosition::~MovePosition()
{
}

void MovePosition::OnBegin()
{
	//m_OwnerCrops->MoveStop( );

	D3DXVECTOR2 destination;
	destination.x = m_Destination.m_EyePoint.x;
	destination.y = m_Destination.m_EyePoint.z;

	ULONGLONG movingTime = m_OwnerCorps->MoveStart( destination );

	m_ActionStatus = ACTION_TICK;
	m_OwnerCorps->DoNextAction( this, movingTime );

}

void MovePosition::OnTick()
{
	// onTick의 역할은?
	LogD( "MovePosition OnTick \n" );

	m_ActionStatus = ACTION_END;
	m_OwnerCorps->DoNextAction( this, 0 );
}

void MovePosition::OnEnd()
{
	LogD( "MovePosition OnEnd \n" );
	m_OwnerCorps->MoveStop();
	Action::OnEnd();
}
