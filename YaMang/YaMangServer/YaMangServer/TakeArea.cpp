#include "stdafx.h"
#include "TakeArea.h"
#include "Corps.h"
#include "GameRoom.h"
#include "PacketType.h"
#include "GuardArea.h"
#include "MacroSet.h"

TakeArea::TakeArea()
{
}


TakeArea::~TakeArea()
{
}

void TakeArea::OnBegin()
{
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
		m_OwnerCorps->DoNextAction( this, m_OwnerCorps->GetAttackDelay() );
	}
	else
	{
		m_OwnerCorps->DoNextAction( this, 0 );
	}
}

void TakeArea::OnTick()
{

	// 둘중 하나라도 죽으면 어텍 취소
	if ( m_OwnerCorps->IsDead() || m_TargetCorps->IsDead() )
	{
		LogD( "TakeArea Failed \n" );
		m_ActionStatus = ACTION_END;
		m_OwnerCorps->DoNextAction( this, 0 );
		return;
	}

	m_OwnerCorps->MoveStop();
	m_TargetCorps->ReCalculatePosition();
	const PositionInfo& myCorpsPositionInfo = m_OwnerCorps->GetPositionInfo( );
	const PositionInfo& targetPositionInfo = m_TargetCorps->GetPositionInfo( );

	D3DXVECTOR2 destination;
	destination.x = targetPositionInfo.m_EyePoint.x;
	destination.y = targetPositionInfo.m_EyePoint.z;
	float length = m_OwnerCorps->GetTargetLength( destination );

	if ( length < m_OwnerCorps->GetAttackRange() )
	{
		m_OwnerCorps->AttackCorps( m_TargetCorps ); 
		
		LogD( "TakeArea OnTick Attack Success \n" );


		if ( m_TargetCorps->IsDead() )
		{
			LogD( "Dead! \n" );
			m_ActionStatus = ACTION_END;
			m_OwnerCorps->DoNextAction( this, 0 );

			UnitType unitType = m_TargetCorps->GetUnitType();
			if ( UnitType::UNIT_GUARD == unitType )
			{
				m_GameRoom->TakeBase( m_OwnerCorps->GetPlayerID( ), m_TargetCorps->GetPlayerID( ), m_OwnerCorps->GetCorpsID( ), m_TargetCorps->GetCorpsID( ) );
			}
			else if ( UnitType::UNIT_KING == unitType )
			{
				m_GameRoom->GameRoomLoose( m_TargetCorps->GetPlayerID() );
			}
			
			
		}
		else
		{
			LogD( "Ready Re Attack!! \n" );

			Action* targetAction = m_TargetCorps->GetHoldingAction();
			if ( !targetAction || ACTION_END == targetAction->GetActionStatus() )
			{
				LogD( "Guard Start! \n" );

				// m_TargerCrops->ChangeFormation( FormationType::FORMATION_DEFENSE );
				GuardArea* action = new GuardArea();
				action->SetGameRoom( m_GameRoom );
				action->SetOwnerCorps( m_TargetCorps );
				action->SetTargetCorps( m_OwnerCorps );

				m_GameRoom->AddActionToScheduler( action, 0 ); // 반격하려고 정신차리는 딜레이 빠른 반격을 위해 여기는 없애자
			}

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
		ULONGLONG movingTime = m_OwnerCorps->MoveStart( destination );

		LogD( "TakeArea OnTick Chase \n" );
		m_ActionStatus = ACTION_TICK;
		m_OwnerCorps->DoNextAction( this, movingTime );
	}
}

void TakeArea::OnEnd()
{
	LogD( "TakeArea OnEnd \n" );
	m_OwnerCorps->MoveStop();
	Action::OnEnd();
}
