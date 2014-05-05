#include "stdafx.h"
#include "Attack.h"
#include "PacketType.h"
#include "Corps.h"
#include "ClientManager.h"

Attack::Attack()
{
}


Attack::~Attack()
{
}

void Attack::OnBegin()
{
	printf_s( "Attack OnBegin \n" );
	m_ActionStatus = ACTION_TICK;
	m_OwnerCrops->DoNextAction( this, 0 );
}

void Attack::OnTick()
{

	// 둘중 하나라도 죽으면 어텍 취소
	if ( m_OwnerCrops->IsDead( ) || m_TargerCrops->IsDead( ) )
	{
		printf_s( "Attack Failed \n" );
		m_ActionStatus = ACTION_END;
		m_OwnerCrops->DoNextAction( this, 0 );
		return;
	}

	PositionInfo myCorpsPositionInfo = m_OwnerCrops->GetPositionInfo();
	PositionInfo targetPositionInfo = m_TargerCrops->GetPositionInfo();

	float nowX = myCorpsPositionInfo.m_EyePoint.x;
	float nowZ = myCorpsPositionInfo.m_EyePoint.z;
	float targetX = targetPositionInfo.m_EyePoint.x;
	float targetZ = targetPositionInfo.m_EyePoint.z;

	D3DXVECTOR2 vector;
	vector.x = targetX - nowX;
	vector.y = targetZ - nowZ;

	float length = D3DXVec2Length( &vector );

	if ( length < m_OwnerCrops->GetAttackRange() )
	{
		

		// 공격 하세요
		// 그냥 stop이 아니라 따로 포메이션 같은걸로 공격 모션, 맞는 모션 있어야 할듯
		// AttackCorpsResult에 적용 예정
		m_TargerCrops->AddDamage( m_OwnerCrops->GetAttackPower( ) );
		StopCorpsResult outPacket;
		outPacket.m_CorpsID = m_OwnerCrops->GetCorpsID( );
		m_ClientManager->BroadcastPacket( &outPacket );

		outPacket.m_CorpsID = m_TargerCrops->GetCorpsID();
		m_ClientManager->BroadcastPacket( &outPacket );

		printf_s( "length:%f  range:%f damage:%f \n", length, m_OwnerCrops->GetAttackRange( ), m_OwnerCrops->GetAttackPower( ) );

		printf_s( "Attack OnTick Attack Success \n" );
		m_ActionStatus = ACTION_END;
		m_OwnerCrops->DoNextAction( this, 0 );
	}
	else
	{
		// 마저 쫓아 가세요
		MoveCorpsResult outPacket;
		outPacket.m_CorpsID = m_OwnerCrops->GetCorpsID();

		D3DXVec2Normalize( &vector, &vector );

		float speed = m_OwnerCrops->GetSpeed();
		float time = ( length / speed ) * 1000;


		outPacket.m_Speed = speed;
		outPacket.m_TargetX = 0.0f;
		outPacket.m_TargetZ = 0.0f;
		outPacket.m_LookX = vector.x;
		outPacket.m_LookZ = vector.y;
		
		PositionInfo position;
		position.m_EyePoint = { targetPositionInfo.m_EyePoint.x, 0.0f, targetPositionInfo.m_EyePoint.z };
		position.m_LookAtPoint = { vector.x, 0.0f, vector.y };

		m_OwnerCrops->SetPositionInfo( position ); // 이거 저장을 onEnd에서 해야 하는데 어떻게 옮길까...
		printf_s( "[ATTACK]m_TargetX:%f m_TargetZ:%f m_LookX:%f m_LookZ:%f \n", outPacket.m_TargetX, outPacket.m_TargetZ, outPacket.m_LookX, outPacket.m_LookZ );

		m_ClientManager->BroadcastPacket( &outPacket );

		printf_s( "Attack OnTick Chase \n" );
		m_ActionStatus = ACTION_TICK;
		m_OwnerCrops->DoNextAction( this, static_cast<ULONGLONG>( time ) );
	}

}

void Attack::OnEnd()
{
	printf_s( "Attack OnEnd \n" );

	Action::OnEnd();
}
