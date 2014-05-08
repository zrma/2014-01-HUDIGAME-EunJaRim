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

	m_OwnerCrops->ReCalculatePosition( );

	MoveCorpsResult outPacket;
	outPacket.m_CorpsID = m_OwnerCrops->GetCorpsID();

	PositionInfo myCorpsPositionInfo = m_OwnerCrops->GetPositionInfo( );
	float nowX = myCorpsPositionInfo.m_EyePoint.x;
	float nowZ = myCorpsPositionInfo.m_EyePoint.z;
	float targetX = m_Destination.m_EyePoint.x;
	float targetZ = m_Destination.m_EyePoint.z;

	D3DXVECTOR2 vector;
	vector.x = targetX - nowX;
	vector.y = targetZ - nowZ;

	float length = D3DXVec2Length( &vector );

	float speed = m_OwnerCrops->GetSpeed();
	outPacket.m_Speed = speed;


	length = D3DXVec2Length( &vector );
	D3DXVec2Normalize( &vector, &vector );

	// target위치를 서버가 계산해줘야함
	outPacket.m_TargetX = targetX;
	outPacket.m_TargetZ = targetZ;
	outPacket.m_LookX = vector.x;
	outPacket.m_LookZ = vector.y;

	
	ULONGLONG movingTime = static_cast<ULONGLONG>( ( length * 1000 ) / speed );
	m_OwnerCrops->MoveStart( movingTime, vector );



	m_ClientManager->BroadcastPacket( &outPacket );
	printf_s( "[MOVE]m_TargetX:%f m_TargetZ:%f m_LookX:%f m_LookZ:%f \n", outPacket.m_TargetX, outPacket.m_TargetZ, outPacket.m_LookX, outPacket.m_LookZ );

	m_ActionStatus = ACTION_TICK;
	m_OwnerCrops->DoNextAction( this, movingTime );

}

void MovePosition::OnTick()
{
	// onTick의 역할은?
	printf_s( "MovePosition OnTick \n" );

	m_ActionStatus = ACTION_END;
	m_OwnerCrops->DoNextAction( this, 0 );
}

void MovePosition::OnEnd()
{
	printf_s( "MovePosition OnEnd \n" );
	m_OwnerCrops->MoveStop();
	Action::OnEnd();
}
