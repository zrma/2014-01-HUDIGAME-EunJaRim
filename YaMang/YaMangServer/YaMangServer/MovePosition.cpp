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
	m_MovingTime = static_cast<ULONGLONG>(( length * 1000 ) / speed);
	outPacket.m_Speed = speed;


	length = D3DXVec2Length( &vector );
	D3DXVec2Normalize( &vector, &vector );

	// target위치를 서버가 계산해줘야함
	outPacket.m_TargetX = targetX;
	outPacket.m_TargetZ = targetZ;
	outPacket.m_LookX = vector.x;
	outPacket.m_LookZ = vector.y;

	printf_s( "[NOW:%f][Target%f:]\n", nowX, targetX );
	m_MovingRoute.m_EyePoint = { nowX, 0.0f, nowZ };
	m_MovingRoute.m_LookAtPoint = { vector.x, 0.0f, vector.y };

	PositionInfo position;
	position.m_EyePoint = { targetX, 0.0f, targetZ };
	position.m_LookAtPoint = { vector.x, 0.0f, vector.y };

	m_OwnerCrops->SetPositionInfo( position );


	// 이걸로 dTime을 구하자
	m_StartedTime = GetTickCount64();

	m_ClientManager->BroadcastPacket( &outPacket );
	printf_s( "[MOVE]m_TargetX:%f m_TargetZ:%f m_LookX:%f m_LookZ:%f \n", outPacket.m_TargetX, outPacket.m_TargetZ, outPacket.m_LookX, outPacket.m_LookZ );

	m_ActionStatus = ACTION_TICK;
	m_OwnerCrops->DoNextAction( this, m_MovingTime );

}

void MovePosition::OnTick()
{
	// onTick의 역할은?
	printf_s( "MovePosition OnTick \n" );

	ULONGLONG elapsedTime = GetTickCount64() - m_StartedTime;
	printf_s( "오차1:%d \n", static_cast<int>( elapsedTime - m_MovingTime ) );

	m_ActionStatus = ACTION_END;
	m_OwnerCrops->DoNextAction( this, 0 );
}

void MovePosition::OnEnd()
{
	printf_s( "MovePosition OnEnd \n" );
	ULONGLONG elapsedTime = GetTickCount64() - m_StartedTime;

	printf_s( "오차2:%d \n", static_cast<int>(elapsedTime - m_MovingTime) );
	// 시간 오차가 0~50ms정도 발생하는데...


	printf_s( "\n [S :%f,%f] [V: %f %f] t : %lu", m_MovingRoute.m_EyePoint.x, m_MovingRoute.m_EyePoint.z,
			  m_MovingRoute.m_LookAtPoint.x, m_MovingRoute.m_LookAtPoint.z, elapsedTime );
	m_MovingRoute.m_EyePoint = m_MovingRoute.m_EyePoint + (m_MovingRoute.m_LookAtPoint * m_OwnerCrops->GetSpeed( ) * elapsedTime / 1000);
	m_OwnerCrops->SetPositionInfo( m_MovingRoute );

	StopCorpsResult outPacket;
	outPacket.m_CorpsID = m_OwnerCrops->GetCorpsID();

	m_ClientManager->BroadcastPacket( &outPacket );

	Action::OnEnd();
}
