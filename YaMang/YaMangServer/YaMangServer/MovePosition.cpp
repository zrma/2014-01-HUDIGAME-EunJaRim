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
	// 이걸로 dTime을 구하자
	m_StartedTime = GetTickCount64( );

	MoveCorpsResult outPacket;
	outPacket.m_CorpsID = m_OwnerCrops->GetCorpsID();

	// 임시로 계산 해둔 식
	D3DXVECTOR3	view = m_Destination.m_LookAtPoint - m_Destination.m_EyePoint;
	float length = D3DXVec3Length( &view );
	float speed = m_OwnerCrops->GetSpeed();
	float time = ( length / speed ) * 1000;


	outPacket.m_Speed = speed;

	// 걸어갈 방향을 지정
	D3DXVec3Normalize( &view, &view );

	// target위치를 서버가 계산해줘야함
	outPacket.m_TargetX = m_Destination.m_LookAtPoint.x;
	outPacket.m_TargetZ = m_Destination.m_LookAtPoint.z;
	outPacket.m_LookX = view.x;
	outPacket.m_LookZ = view.z;

	PositionInfo position;
	position.m_EyePoint = { m_Destination.m_LookAtPoint.x, 0.0f, m_Destination.m_LookAtPoint.z };
	position.m_LookAtPoint = { view.x, 0.0f, view.z };

	m_OwnerCrops->SetPositionInfo( position );

	m_ClientManager->BroadcastPacket( &outPacket );
	
	printf_s( "MovePosition OnBegin [time:%f][%f][%f][%f][%f] \n", time, m_Destination.m_LookAtPoint.x, m_Destination.m_LookAtPoint.z, view.x, view.z );
	m_ActionStatus = ACTION_TICK;
	m_OwnerCrops->DoNextAction( this, static_cast<ULONGLONG>(time) );
}

void MovePosition::OnTick()
{
	// onTick의 역할은?
	printf_s( "MovePosition OnTick \n" );
	m_ActionStatus = ACTION_END;
	m_OwnerCrops->DoNextAction( this, 10 );
}

void MovePosition::OnEnd()
{
	printf_s( "MovePosition OnEnd \n" );

	StopCorpsResult outPacket;
	outPacket.m_CorpsID = m_OwnerCrops->GetCorpsID();

	m_ClientManager->BroadcastPacket( NULL ,&outPacket );

	Action::OnEnd();
}
