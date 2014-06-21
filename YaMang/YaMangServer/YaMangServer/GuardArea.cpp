#include "stdafx.h"
#include "GuardArea.h"
#include "PacketType.h"
#include "Corps.h"
#include "GameRoom.h"
#include "MacroSet.h"
#include "Attack.h"

GuardArea::GuardArea()
{
}


GuardArea::~GuardArea()
{
}

void GuardArea::SetTargetCorps( Corps* targetCrops )
{
	m_TargetCorps = targetCrops;
	m_GuardModeOn = true;
}

void GuardArea::OnBegin()
{
	LogD( "GuardArea OnBegin \n" );
	m_ActionStatus = ACTION_TICK;

	m_OwnerCorps->ReCalculatePosition();
	m_TargetCorps->ReCalculatePosition();

	if ( !m_GuardModeOn )
	{
		m_OwnerCorps->DoNextAction( this, 0 );
		return;
	}


	const PositionInfo& targetPositionInfo = m_TargetCorps->GetPositionInfo();

	D3DXVECTOR2 destination;
	destination.x = targetPositionInfo.m_EyePoint.x;
	destination.y = targetPositionInfo.m_EyePoint.z;
	float length = m_OwnerCorps->GetTargetLength( destination );

	// 공격명령이 바로 앞에서 지시될때와 이동해야할 때를 구분 
	if ( length < m_OwnerCorps->GetAttackRange() )
	{
		m_OwnerCorps->DoNextAction( this, m_OwnerCorps->GetAttackDelay() );
	}
	else
	{
		m_OwnerCorps->DoNextAction( this, 0 );
	}

}

void GuardArea::OnTick()
{
	// 둘중 하나라도 죽으면 어텍 취소
	if ( m_OwnerCorps->IsDead() || m_TargetCorps->IsDead() )
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

	float nowX = myCorpsPositionInfo.m_EyePoint.x;
	float nowZ = myCorpsPositionInfo.m_EyePoint.z;
	float targetX = targetPositionInfo.m_EyePoint.x;
	float targetZ = targetPositionInfo.m_EyePoint.z;


	D3DXVECTOR2 destination;
	destination.x = targetPositionInfo.m_EyePoint.x;
	destination.y = targetPositionInfo.m_EyePoint.z;
	float length = m_OwnerCorps->GetTargetLength( destination );



	if ( m_GuardModeOn && length < m_OwnerCorps->GetAttackRange( ) )
	{
		m_OwnerCorps->AttackCorps( m_TargetCorps );

		LogD( "GuardArea OnTick Attack Success \n" );


		if ( m_TargetCorps->IsDead() )
		{
			LogD( "Guarder Killed Target! \n" );
			ReturnMyBase();
			return;
		}
		else if ( m_OwnerCorps->IsDead() )
		{
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
		PositionInfo originalPosition = GetOriginalPositionInfo();
		D3DXVECTOR2 originalPositionVector;
		originalPositionVector.x = originalPosition.m_EyePoint.x - nowX;
		originalPositionVector.y = originalPosition.m_EyePoint.z - nowZ;
		length = D3DXVec2Length( &originalPositionVector );

		// 하드 코딩 쫓아가는것을 포기하는 거리
		if ( length > 25 )
		{
			LogD( "Too far to chase \n" );
			ReturnMyBase();
			return;
		}

		// 마저 쫓아 가세요
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


		LogD( "GuardArea OnTick Chase \n" );
		m_ActionStatus = ACTION_TICK;
		m_OwnerCorps->DoNextAction( this, movingTime );



	}
}

void GuardArea::OnEnd()
{
	LogD( "GuardArea OnEnd \n" );
	m_OwnerCorps->MoveStop();
	Action::OnEnd();
}

void GuardArea::ReturnMyBase()
{
	LogD( "Return to my original Position! \n" );

	const PositionInfo& targetPositionInfo = m_TargetCorps->GetPositionInfo();


	
	PositionInfo originalPosition = GetOriginalPositionInfo();

	
	m_GuardModeOn = false;


	D3DXVECTOR2 destination;
	destination.x = originalPosition.m_EyePoint.x + 0.01f;
	destination.y = originalPosition.m_EyePoint.z + 0.01f;
	ULONGLONG movingTime = m_OwnerCorps->MoveStart( destination );

	LogD( "GuardArea OnTick Return to my original Position \n" );
	m_ActionStatus = ACTION_END;
	m_OwnerCorps->DoNextAction( this, movingTime );
}

const PositionInfo GuardArea::GetOriginalPositionInfo() const
{
	UnitType unitType = m_OwnerCorps->GetUnitType();

	PositionInfo originalPosition;
	if ( UnitType::UNIT_GUARD == unitType )
	{
		int guardIndex = m_GameRoom->GetGuardIndexByID( m_OwnerCorps->GetCorpsID() );
		originalPosition = m_GameRoom->GetGuardPositionInfo( guardIndex );
	}
	else if ( UnitType::UNIT_KING == unitType )
	{
		int kingIndex = m_GameRoom->GetKingIndexByID( m_OwnerCorps->GetCorpsID() );
		originalPosition = m_GameRoom->GetKingPositionInfo( kingIndex );
	}
	return originalPosition;
}
