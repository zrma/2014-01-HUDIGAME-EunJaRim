#include "stdafx.h"
#include "MovePosition.h"
#include "Action.h"
#include "ActionScheduler.h"
#include "PacketType.h"
#include "ClientManager.h"


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
	m_ActionScheduler->AddActionToScheduler( this, 1000 );
}

void MovePosition::OnTick()
{
	printf_s( "MovePosition OnTick \n" );
	m_ActionStatus = ACTION_END;
	m_ActionScheduler->AddActionToScheduler( this, 1000 );
}

void MovePosition::OnEnd()
{
	printf_s( "MovePosition OnEnd \n" );
	// 종료 패킷 브로드 캐스팅 해야함
	// 그런데 어디서 클라매니저를 갖고올까 ㅇㅅㅇ;

	StopCorpsResult outPacket;
	outPacket.m_CorpsID = m_OwnerCropsID;

	m_ClientManager->BroadcastPacket( NULL ,&outPacket );

	Action::OnEnd();
}
