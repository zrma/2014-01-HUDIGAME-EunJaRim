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


	// length를 구하기 위한 중복이지만 한번정도는 괜찮겠지...
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

	// 공격명령이 바로 앞에서 지시될때와 이동해야할 때를 구분 
	if ( length < m_OwnerCrops->GetAttackRange() )
	{
		m_OwnerCrops->DoNextAction( this, m_OwnerCrops->GetAttackDelay( ) );
	}
	else
	{
		m_OwnerCrops->DoNextAction( this, 0 );
	}
	
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
		// attack result packet 보내기
		m_TargerCrops->AddDamage( m_OwnerCrops->GetAttackPower( ) );

		AttackCorpsResult outPacket;
		outPacket.m_AttackingCorpsID = m_OwnerCrops->GetCorpsID( );
		outPacket.m_TargetCorpsID = m_TargerCrops->GetCorpsID();

		float targetHP = m_TargerCrops->GetHP() + 9;
		int targetUnitNum = static_cast<int>( targetHP / 10 );
		outPacket.m_TargetUnitNum = targetUnitNum;


		m_ClientManager->BroadcastPacket( &outPacket );

		printf_s( "[Attack] length:%f  range:%f damage:%f \n", length, m_OwnerCrops->GetAttackRange( ), m_OwnerCrops->GetAttackPower( ) );
		printf_s( "Attack OnTick Attack Success \n" );


		if ( m_OwnerCrops->IsDead() || m_TargerCrops->IsDead() )
		{
			printf_s( "Dead! \n" );
			m_ActionStatus = ACTION_END;
			m_OwnerCrops->DoNextAction( this, 0 );
		}
		else
		{
			printf_s( "Ready Re Attack!! \n" );
			m_ActionStatus = ACTION_TICK;
			m_OwnerCrops->DoNextAction( this, m_OwnerCrops->GetAttackDelay() );
		}
		


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
