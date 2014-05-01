#include "stdafx.h"
#include "MovePosition.h"
#include "Action.h"
#include "ActionScheduler.h"
#include "PacketType.h"
#include "ClientManager.h"
#include "Corps.h"


MovePosition::MovePosition()
{
}

MovePosition::~MovePosition()
{
}

void MovePosition::OnBegin()
{
	printf_s( "MovePosition OnBegin \n" );
	m_ActionStatus = ACTION_TICK;
	m_OwnerCrops->DoNextAction( this, 1000 );
}

void MovePosition::OnTick()
{
	printf_s( "MovePosition OnTick \n" );
	m_ActionStatus = ACTION_END;
	m_OwnerCrops->DoNextAction( this, 1000 );
}

void MovePosition::OnEnd()
{
	printf_s( "MovePosition OnEnd \n" );

	StopCorpsResult outPacket;
	outPacket.m_CorpsID = m_OwnerCrops->GetCorpsID();

	m_ClientManager->BroadcastPacket( NULL ,&outPacket );

	Action::OnEnd();
}
