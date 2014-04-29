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
	
	///# 보통 여기서 move_start 패킷 보내준다. (방향, 속도 목적지)

	m_ActionStatus = ACTION_TICK;

	///# 액션이 액션 스케줄러의 포인터를 들고 있는 것은 좋은 디자인이 아니다. 보통은 이 액션의 주체가 되는 Owner(corp..) 포인터를 들고 있고
	/// 그 오너에게 mOwner->DoNextAction()식으로 처리하도록 해주는게 좋다.
	m_ActionScheduler->AddActionToScheduler( this, 1000 );

	
}

void MovePosition::OnTick()
{
	printf_s( "MovePosition OnTick \n" );

	///# 보통 여기서 move_tick_update 패킷 보내준다. (방향, 속도 목적지)
	///# 만일 길찾기가 수반된 이동이라면, 변침할때마다 move_tick_update를 보내주도록 한다. 
	///# 즉, 다음 상태는 [ACTION_TICK]+ 

	m_ActionStatus = ACTION_END;
	m_ActionScheduler->AddActionToScheduler( this, 1000 );
}

void MovePosition::OnEnd()
{
	printf_s( "MovePosition OnEnd \n" );

	StopCorpsResult outPacket;
	outPacket.m_CorpsID = m_OwnerCropsID;

	///# 보통 여기서 move_end 패킷 보내준다. (최종 위치)

	m_ClientManager->BroadcastPacket( NULL ,&outPacket );

	Action::OnEnd();
}
