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

	ULONGLONG movingTime = m_OwnerCrops->MoveStart( destination );

	m_ActionStatus = ACTION_TICK;
	m_OwnerCrops->DoNextAction( this, movingTime );

}

void MovePosition::OnTick()
{
	// onTick의 역할은?
	Log( "MovePosition OnTick \n" );

	m_ActionStatus = ACTION_END;
	m_OwnerCrops->DoNextAction( this, 0 );
}

void MovePosition::OnEnd()
{
	Log( "MovePosition OnEnd \n" );
	m_OwnerCrops->MoveStop();
	Action::OnEnd();
}
