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
	m_TargerCrops->ReCalculatePosition();


	const PositionInfo& targetPositionInfo = m_TargerCrops->GetPositionInfo();

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
	if ( m_OwnerCrops->IsDead() || m_TargerCrops->IsDead() )
	{
		Log( "TakeArea Failed \n" );
		m_ActionStatus = ACTION_END;
		m_OwnerCrops->DoNextAction( this, 0 );
		return;
	}

	m_OwnerCrops->MoveStop();
	m_TargerCrops->ReCalculatePosition();
	const PositionInfo& myCorpsPositionInfo = m_OwnerCrops->GetPositionInfo( );
	const PositionInfo& targetPositionInfo = m_TargerCrops->GetPositionInfo( );

	D3DXVECTOR2 destination;
	destination.x = targetPositionInfo.m_EyePoint.x;
	destination.y = targetPositionInfo.m_EyePoint.z;
	float length = m_OwnerCrops->GetTargetLength( destination );

	if ( length < m_OwnerCrops->GetAttackRange() )
	{
		m_OwnerCrops->AttackCorps( m_TargerCrops ); 
		
		Log( "TakeArea OnTick Attack Success \n" );


		if ( m_TargerCrops->IsDead() )
		{
			Log( "Dead! \n" );
			m_ActionStatus = ACTION_END;
			m_OwnerCrops->DoNextAction( this, 0 );

			UnitType unitType = m_TargerCrops->GetUnitType();
			if ( UnitType::UNIT_GUARD == unitType )
			{
				m_ClientManager->TakeBase( m_OwnerCrops->GetPlayerID( ), m_TargerCrops->GetPlayerID( ), m_OwnerCrops->GetCorpsID( ), m_TargerCrops->GetCorpsID( ) );
			}
			else if ( UnitType::UNIT_KING == unitType )
			{
				m_ClientManager->GameRoomLoose( m_TargerCrops->GetPlayerID() );
			}
			
			
		}
		else
		{
			Log( "Ready Re Attack!! \n" );

			Action* targetAction = m_TargerCrops->GetHoldingAction();
			if ( !targetAction || ACTION_END == targetAction->GetActionStatus() )
			{
				Log( "Guard Start! \n" );

				// m_TargerCrops->ChangeFormation( FormationType::FORMATION_DEFENSE );
				GuardArea* action = new GuardArea();
				action->SetClientManager( m_ClientManager );
				action->SetOwnerCorps( m_TargerCrops );
				action->SetTargetCorps( m_OwnerCrops );

				m_ClientManager->AddActionToScheduler( action, 0 ); // 반격하려고 정신차리는 딜레이 빠른 반격을 위해 여기는 없애자
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

		Log( "TakeArea OnTick Chase \n" );
		m_ActionStatus = ACTION_TICK;
		m_OwnerCrops->DoNextAction( this, movingTime );
	}
}

void TakeArea::OnEnd()
{
	Log( "TakeArea OnEnd \n" );
	m_OwnerCrops->MoveStop();
	Action::OnEnd();
}
