#include "stdafx.h"
#include "MovePosition.h"
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

	MoveCorpsResult outPacket;
	outPacket.m_CorpsID = m_OwnerCrops->GetCorpsID();

	// 임시로 계산 해둔 식
	D3DXVECTOR3	view = m_Destination.m_LookAtPoint - m_Destination.m_EyePoint;
	float speed = D3DXVec3Length( &view );
	outPacket.m_Speed = speed;

	// 걸어갈 방향을 지정
	D3DXVec3Normalize( &view, &view );

	// target위치를 서버가 계산해줘야함
	outPacket.m_TargetX = m_Destination.m_LookAtPoint.x;
	outPacket.m_TargetZ = m_Destination.m_LookAtPoint.z;
	outPacket.m_LookX = view.x;
	outPacket.m_LookZ = view.z;

	m_ClientManager->BroadcastPacket( &outPacket );


	printf_s( "MovePosition OnBegin \n" );
	m_ActionStatus = ACTION_TICK;
	m_OwnerCrops->DoNextAction( this, 1500 );
}

void MovePosition::OnTick()
{
	printf_s( "MovePosition OnTick \n" );
	m_ActionStatus = ACTION_END;
	m_OwnerCrops->DoNextAction( this, 1500 );
}

void MovePosition::OnEnd()
{
	printf_s( "MovePosition OnEnd \n" );

	StopCorpsResult outPacket;
	outPacket.m_CorpsID = m_OwnerCrops->GetCorpsID();

	m_ClientManager->BroadcastPacket( NULL ,&outPacket );

	Action::OnEnd();
}
