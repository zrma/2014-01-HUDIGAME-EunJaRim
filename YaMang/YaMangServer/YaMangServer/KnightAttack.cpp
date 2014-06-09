#include "stdafx.h"
#include "KnightAttack.h"
#include "Attack.h"
#include "PacketType.h"
#include "Corps.h"
#include "GameRoom.h"
#include "MacroSet.h"

KnightAttack::KnightAttack()
{
}


KnightAttack::~KnightAttack()
{
}

void KnightAttack::OnBegin()
{
	LogD( "Knight Attack OnBegin \n" );
	m_ActionStatus = ACTION_TICK;
	m_OwnerCrops->DoNextAction( this, 0 );
}

void KnightAttack::OnTick()
{

	// 둘중 하나라도 죽으면 어텍 취소
	if ( m_OwnerCrops->IsDead() || m_TargetCrops->IsDead() )
	{
		LogD( "Knight Attack Failed \n" );
		m_ActionStatus = ACTION_END;
		m_OwnerCrops->DoNextAction( this, 0 );
		return;
	}

	m_OwnerCrops->MoveStop();
	m_TargetCrops->ReCalculatePosition();
	const PositionInfo& myCorpsPositionInfo = m_OwnerCrops->GetPositionInfo();
	const PositionInfo& targetPositionInfo = m_TargetCrops->GetPositionInfo();



	D3DXVECTOR2 destination;
	destination.x = targetPositionInfo.m_EyePoint.x;
	destination.y = targetPositionInfo.m_EyePoint.z;
	float length = m_OwnerCrops->GetTargetLength( destination );


	// 하드 코딩 더 움직이는 거리
	float moveMoreDistance = 25;

	// 하드 코딩 반환해도 되는 거리
	if ( length > 5 )
	{
		m_CanAttack = true;

	}

	if ( m_CanAttack && length < m_OwnerCrops->GetAttackRange( ) )
	{
		// 지연 공격 방어
		ULONGLONG eTime = GetTickCount64() - m_AttackedTime;
		if ( eTime < m_OwnerCrops->GetAttackDelay() )
		{
			m_ActionStatus = ACTION_TICK;
			m_OwnerCrops->DoNextAction( this, m_OwnerCrops->GetAttackDelay( ) - eTime );
			return;
		}

		m_OwnerCrops->AttackCorps( m_TargetCrops );

		LogD( "Knight Attack OnTick Attack Success \n" );


		if ( m_OwnerCrops->IsDead() || m_TargetCrops->IsDead() )
		{
			LogD( "Dead! \n" );
			m_ActionStatus = ACTION_END;
			m_OwnerCrops->DoNextAction( this, 0 );
			return;
		}

		m_CanAttack = false;
		m_AttackedTime = GetTickCount64( );

		m_ActionStatus = ACTION_TICK;
		m_OwnerCrops->DoNextAction( this, 0 );
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

		if ( !m_CanAttack )
		{
			LogD( "move more \n" );
			
			if ( vector.x > 0 )
			{
				targetX = targetX + moveMoreDistance;
			}
			else
			{
				targetX = targetX - moveMoreDistance;
			}
			if ( vector.y > 0 )
			{
				targetZ = targetZ + moveMoreDistance;
			}
			else
			{
				targetZ = targetZ - moveMoreDistance;
			}
		}

		D3DXVECTOR2 destination;
		destination.x = targetX;
		destination.y = targetZ;
		ULONGLONG movingTime = m_OwnerCrops->MoveStart( destination, 2 );


		LogD( "Knight Attack OnTick Chase \n" );
		m_ActionStatus = ACTION_TICK;
		m_OwnerCrops->DoNextAction( this, movingTime );
	}
}

void KnightAttack::OnEnd()
{
	LogD( "Knight Attack OnEnd \n" );
	m_OwnerCrops->MoveStop();
	Action::OnEnd();
}
