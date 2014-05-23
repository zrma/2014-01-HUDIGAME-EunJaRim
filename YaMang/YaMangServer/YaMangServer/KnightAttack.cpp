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
	Log( "Knight Attack OnBegin \n" );
	m_ActionStatus = ACTION_TICK;
	m_OwnerCrops->DoNextAction( this, 0 );
}

void KnightAttack::OnTick()
{

	// 둘중 하나라도 죽으면 어텍 취소
	if ( m_OwnerCrops->IsDead() || m_TargerCrops->IsDead() )
	{
		Log( "Knight Attack Failed \n" );
		m_ActionStatus = ACTION_END;
		m_OwnerCrops->DoNextAction( this, 0 );
		return;
	}

	m_OwnerCrops->MoveStop();
	m_TargerCrops->ReCalculatePosition();
	const PositionInfo& myCorpsPositionInfo = m_OwnerCrops->GetPositionInfo();
	const PositionInfo& targetPositionInfo = m_TargerCrops->GetPositionInfo();



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
		ULONGLONG eTime = GetTickCount64() - m_AttackedTime;
		if ( eTime < m_OwnerCrops->GetAttackDelay() )
		{
			m_ActionStatus = ACTION_TICK;
			m_OwnerCrops->DoNextAction( this, m_OwnerCrops->GetAttackDelay( ) - eTime );
			return;
		}

		m_TargerCrops->MoveStop();
		Action* targetAction = m_TargerCrops->GetHoldingAction();

		// targetCorps의 액션이 없으면(idle)이면 반격
		// 아니면 그냥 무시하고 계속 진행
		if ( nullptr == targetAction || ACTION_END == targetAction->GetActionStatus() )
		{
			Log( "target CounterAttack! \n" );
			//m_TargerCrops->ChangeFormation( FormationType::FORMATION_DEFENSE );// 망진으로 변경해야함
			Attack* action = new Attack();
			action->SetClientManager( m_ClientManager );
			action->SetOwnerCorps( m_TargerCrops );
			action->SetTargetCorps( m_OwnerCrops );

			m_ClientManager->AddActionToScheduler( action, m_TargerCrops->GetAttackDelay() / 3 ); // 반격하려고 정신차리는 딜레이
		}


		// 공격 하세요
		// attack result packet 보내기
		m_TargerCrops->AddDamage( m_OwnerCrops->GetAttackPower() );

		AttackCorpsResult outPacket;
		outPacket.m_AttackerCorpsID = m_OwnerCrops->GetCorpsID();
		outPacket.m_TargetCorpsID = m_TargerCrops->GetCorpsID();
		outPacket.m_AttackerNowX = myCorpsPositionInfo.m_EyePoint.x;
		outPacket.m_AttackerNowZ = myCorpsPositionInfo.m_EyePoint.z;
		outPacket.m_AttackerLookX = myCorpsPositionInfo.m_LookAtPoint.x;
		outPacket.m_AttackerLookZ = myCorpsPositionInfo.m_LookAtPoint.z;

		outPacket.m_TargetNowX = targetPositionInfo.m_EyePoint.x;
		outPacket.m_TargetNowZ = targetPositionInfo.m_EyePoint.z;
		outPacket.m_TargetLookX = targetPositionInfo.m_LookAtPoint.x;
		outPacket.m_TargetLookZ = targetPositionInfo.m_LookAtPoint.z;

		outPacket.m_TargetUnitNum = m_TargerCrops->GetUnitNum();


		m_ClientManager->BroadcastPacket( &outPacket );

		Log( "[Knight Attack] length:%f  range:%f damage:%f \n", length, m_OwnerCrops->GetAttackRange(), m_OwnerCrops->GetAttackPower() );
		Log( "[Knight Attack] Attacker:[%f][%f] Defencer:[%f][%f] \n", m_OwnerCrops->GetPositionInfo().m_EyePoint.x, m_OwnerCrops->GetPositionInfo().m_EyePoint.z, m_TargerCrops->GetPositionInfo().m_EyePoint.x, m_TargerCrops->GetPositionInfo().m_EyePoint.z );
		Log( "Knight Attack OnTick Attack Success \n" );


		if ( m_OwnerCrops->IsDead() || m_TargerCrops->IsDead() )
		{
			Log( "Dead! \n" );
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
			Log( "move more \n" );
			
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
		ULONGLONG movingTime = m_OwnerCrops->MoveStart2( destination, 2 );


		Log( "Knight Attack OnTick Chase \n" );
		m_ActionStatus = ACTION_TICK;
		m_OwnerCrops->DoNextAction( this, movingTime );
	}
}

void KnightAttack::OnEnd()
{
	Log( "Knight Attack OnEnd \n" );
	m_OwnerCrops->MoveStop();
	Action::OnEnd();
}
