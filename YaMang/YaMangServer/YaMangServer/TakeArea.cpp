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
	m_OwnerCrops->ReCalculatePosition();
	m_TargetCrops->ReCalculatePosition();


	const PositionInfo& targetPositionInfo = m_TargetCrops->GetPositionInfo();

	D3DXVECTOR2 destination;
	destination.x = targetPositionInfo.m_EyePoint.x;
	destination.y = targetPositionInfo.m_EyePoint.z;
	float length = m_OwnerCrops->GetTargetLength( destination );

	m_ActionStatus = ACTION_TICK;
	// 공격명령이 바로 앞에서 지시될때와 이동해야할 때를 구분 
	if ( length < m_OwnerCrops->GetAttackRange() )
	{
		m_OwnerCrops->DoNextAction( this, m_OwnerCrops->GetAttackDelay() );
	}
	else
	{
		m_OwnerCrops->DoNextAction( this, 0 );
	}
}

void TakeArea::OnTick()
{

	// 둘중 하나라도 죽으면 어텍 취소
	if ( m_OwnerCrops->IsDead() || m_TargetCrops->IsDead() )
	{
		LogD( "TakeArea Failed \n" );
		m_ActionStatus = ACTION_END;
		m_OwnerCrops->DoNextAction( this, 0 );
		return;
	}

	m_OwnerCrops->MoveStop();
	m_TargetCrops->ReCalculatePosition();
	const PositionInfo& myCorpsPositionInfo = m_OwnerCrops->GetPositionInfo( );
	const PositionInfo& targetPositionInfo = m_TargetCrops->GetPositionInfo( );

	D3DXVECTOR2 destination;
	destination.x = targetPositionInfo.m_EyePoint.x;
	destination.y = targetPositionInfo.m_EyePoint.z;
	float length = m_OwnerCrops->GetTargetLength( destination );

	if ( length < m_OwnerCrops->GetAttackRange() )
	{
		m_OwnerCrops->AttackCorps( m_TargetCrops ); 
		
		LogD( "TakeArea OnTick Attack Success \n" );


		if ( m_TargetCrops->IsDead() )
		{
			LogD( "Dead! \n" );
			m_ActionStatus = ACTION_END;
			m_OwnerCrops->DoNextAction( this, 0 );

			UnitType unitType = m_TargetCrops->GetUnitType();
			if ( UnitType::UNIT_GUARD == unitType )
			{
				m_GameRoom->TakeBase( m_OwnerCrops->GetPlayerID( ), m_TargetCrops->GetPlayerID( ), m_OwnerCrops->GetCorpsID( ), m_TargetCrops->GetCorpsID( ) );
			}
			else if ( UnitType::UNIT_KING == unitType )
			{
				m_GameRoom->GameRoomLoose( m_TargetCrops->GetPlayerID() );
			}
			
			
		}
		else
		{
			LogD( "Ready Re Attack!! \n" );

			Action* targetAction = m_TargetCrops->GetHoldingAction();
			if ( !targetAction || ACTION_END == targetAction->GetActionStatus() )
			{
				LogD( "Guard Start! \n" );

				// m_TargerCrops->ChangeFormation( FormationType::FORMATION_DEFENSE );
				GuardArea* action = new GuardArea();
				action->SetGameRoom( m_GameRoom );
				action->SetOwnerCorps( m_TargetCrops );
				action->SetTargetCorps( m_OwnerCrops );

				m_GameRoom->AddActionToScheduler( action, 0 ); // 반격하려고 정신차리는 딜레이 빠른 반격을 위해 여기는 없애자
			}

			m_ActionStatus = ACTION_TICK;
			m_OwnerCrops->DoNextAction( this, m_OwnerCrops->GetAttackDelay() );
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

		float halfRange = m_OwnerCrops->GetAttackRange() / 2;
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
		ULONGLONG movingTime = m_OwnerCrops->MoveStart( destination );

		LogD( "TakeArea OnTick Chase \n" );
		m_ActionStatus = ACTION_TICK;
		m_OwnerCrops->DoNextAction( this, movingTime );
	}
}

void TakeArea::OnEnd()
{
	LogD( "TakeArea OnEnd \n" );
	m_OwnerCrops->MoveStop();
	Action::OnEnd();
}
