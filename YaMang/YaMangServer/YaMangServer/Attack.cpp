#include "stdafx.h"
#include "Attack.h"
#include "PacketType.h"
#include "Corps.h"
#include "GameRoom.h"
#include "MacroSet.h"
Attack::Attack()
{
}


Attack::~Attack()
{
}

void Attack::OnBegin()
{
	LogD( "Attack OnBegin \n" );
	m_OwnerCorps->ReCalculatePosition();
	m_TargetCorps->ReCalculatePosition();


	const PositionInfo& targetPositionInfo = m_TargetCorps->GetPositionInfo();

	D3DXVECTOR2 destination;
	destination.x = targetPositionInfo.m_EyePoint.x;
	destination.y = targetPositionInfo.m_EyePoint.z;
	float length = m_OwnerCorps->GetTargetLength( destination );

	m_ActionStatus = ACTION_TICK;
	// 공격명령이 바로 앞에서 지시될때와 이동해야할 때를 구분 
	if ( length < m_OwnerCorps->GetAttackRange() )
	{
		m_OwnerCorps->DoNextAction( this, m_OwnerCorps->GetAttackDelay( ) );
	}
	else
	{
		m_OwnerCorps->DoNextAction( this, 0 );
	}
	
}

void Attack::OnTick()
{

	// 둘중 하나라도 죽으면 어텍 취소
	if ( m_OwnerCorps->IsDead( ) || m_TargetCorps->IsDead( ) )
	{
		LogD( "Attack Failed \n" );
		m_ActionStatus = ACTION_END;
		m_OwnerCorps->DoNextAction( this, 0 );
		return;
	}

	m_OwnerCorps->MoveStop();
	m_TargetCorps->ReCalculatePosition();
	
	const PositionInfo& myCorpsPositionInfo = m_OwnerCorps->GetPositionInfo();
	const PositionInfo& targetPositionInfo = m_TargetCorps->GetPositionInfo();
	

	D3DXVECTOR2 destination;
	destination.x = targetPositionInfo.m_EyePoint.x;
	destination.y = targetPositionInfo.m_EyePoint.z;
	float length = m_OwnerCorps->GetTargetLength( destination );

	if ( length < m_OwnerCorps->GetAttackRange() )
	{
		
		m_OwnerCorps->AttackCorps( m_TargetCorps );

		LogD( "Attack OnTick Attack Success \n" );


		if ( m_OwnerCorps->IsDead() || m_TargetCorps->IsDead() )
		{
			LogD( "Dead! \n" );
			m_ActionStatus = ACTION_END;
			m_OwnerCorps->DoNextAction( this, 0 );
		}
		else
		{
			LogD( "Ready Re Attack!! \n" );
			m_ActionStatus = ACTION_TICK;
			m_OwnerCorps->DoNextAction( this, m_OwnerCorps->GetAttackDelay() );
		}
	}
	else
	{
		// 마저 쫓아 가세요

		float nowX = myCorpsPositionInfo.m_EyePoint.x;
		float nowZ = myCorpsPositionInfo.m_EyePoint.z;
		float targetX = targetPositionInfo.m_EyePoint.x;
		float targetZ = targetPositionInfo.m_EyePoint.z;
		
		D3DXVECTOR2 vector;
		vector.x = targetX - nowX;
		vector.y = targetZ - nowZ;

		float halfRange = m_OwnerCorps->GetAttackRange() / 2;
		if ( vector.x > 0 )
		{
			targetX = targetX - halfRange;
		}
		else
		{
			targetX = targetX + halfRange;
		}
		if ( vector.y > 0 )
		{
			targetZ = targetZ - halfRange;
		}
		else
		{
			targetZ = targetZ + halfRange;
		}


		D3DXVECTOR2 destination;
		destination.x = targetX;
		destination.y = targetZ;
		ULONGLONG movingTime = m_OwnerCorps->MoveStart( destination, 2 );


		LogD( "Attack OnTick Chase \n" );
		m_ActionStatus = ACTION_TICK;
		m_OwnerCorps->DoNextAction( this, movingTime );
	}
}

void Attack::OnEnd()
{
	LogD( "Attack OnEnd \n" );
	m_OwnerCorps->MoveStop( );
	Action::OnEnd();
}
