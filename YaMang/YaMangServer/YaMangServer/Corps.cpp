#include "stdafx.h"
#include "Corps.h"
#include "Arrow.h"
#include "Guard.h"
#include "Sword.h"
#include "Pike.h"
#include "knight.h"
#include "EnumSet.h"
#include "PacketType.h"
#include "GameRoom.h"
#include "Action.h"
#include "RoomManager.h"
#include "ClientSession.h"
#include "MovePosition.h"
#include "MacroSet.h"
#include "Attack.h"

Corps::Corps( int playerID, int corpsID, UnitType unitType, PositionInfo position, GameRoom* gameRoom )
: m_PlayerID( playerID ), m_CorpsID( corpsID ), m_UnitType(unitType), m_Position( position ), m_GameRoom( gameRoom )
{
	D3DXVECTOR3 view = m_Position.m_LookAtPoint - m_Position.m_EyePoint;
	m_MovingRoute.m_EyePoint = { m_Position.m_EyePoint.x, 0.0f, m_Position.m_EyePoint.z };
	m_MovingRoute.m_LookAtPoint = { view.x, 0.0f, view.y };
}


Corps::~Corps()
{
}


void Corps::SetHP( float HP )
{
	m_HP = HP;
	CalculateHP();
}


void Corps::AddDamage( float damage )
{
	if ( !m_IsDead )
	{
		float defense = m_Defense + m_DefenseBonus - damage;
		if ( defense < 0 )
		{
			m_HP = m_HP + defense;
			if ( m_HP < 1 )
			{
				m_IsDead = true;
				auto it = g_PidSessionTable.find( m_PlayerID );
				if ( it != g_PidSessionTable.end() )
				{
					ClientSession* client = it->second;
					client->SubCorpsNum();
				}
			}
		}
		
	}

	CalculateHP();
}

void Corps::ChangeFormation( FormationType formation )
{
// 	if ( m_HoldAction )
// 	{
// 		m_HoldAction->LowKick( );
// 	}
// 	D3DXVECTOR2 destination;
// 	destination.x = m_Position.m_EyePoint.x;
// 	destination.y = m_Position.m_EyePoint.z;
// 	MoveStart( destination );


	if ( m_HoldAction )
	{
		ReCalculatePosition();
		PositionInfo destination;
		destination.m_EyePoint = { m_Position.m_EyePoint.x, 0.0f, m_Position.m_EyePoint.z };
		destination.m_LookAtPoint = { m_Position.m_EyePoint.x + m_Position.m_LookAtPoint.x, 0.0f, m_Position.m_EyePoint.z + m_Position.m_LookAtPoint.z };
		destination.m_EyePoint += ( m_MovingRoute.m_LookAtPoint * 10);

		MovePosition* action = new MovePosition();
		action->SetGameRoom( m_GameRoom );
		action->SetOwnerCorps( this );
		action->SetDestination( destination );

		m_GameRoom->AddActionToScheduler( action, 0 );
	}


	m_Formation = formation;

	switch ( formation )
	{
		case FormationType::FORMATION_DEFENSE:
			m_MoveSpeedBonus = 0.0f;
			m_AttackRangeBonus = 0.0f;
			m_AttackPowerBonus = 0.0f;
			m_DefenseBonus = 2.0f;
			m_AttackDelayBonus = 0;
			break;
		case FormationType::FORMATION_DESTROY:
			m_MoveSpeedBonus = 0.0f;
			m_AttackRangeBonus = 0.0f;
			m_AttackPowerBonus = 2.0f;
			m_DefenseBonus = 0.0f;
			m_AttackDelayBonus = 0;
			break;
		case FormationType::FORMATION_RUSH:
			m_MoveSpeedBonus = 2.0f;
			m_AttackRangeBonus = 0.0f;
			m_AttackPowerBonus = 0.0f;
			m_DefenseBonus = 0.0f;
			m_AttackDelayBonus = 0;
			break;
		default:
			m_MoveSpeedBonus = 0.0f;
			m_AttackRangeBonus = 0.0f;
			m_AttackPowerBonus = 0.0f;
			m_DefenseBonus = 0.0f;
			m_AttackDelayBonus = 0;
			break;
	}



	ChangeCorpsFormationResult outPacket;
	outPacket.m_CorpsID = m_CorpsID;
	outPacket.m_FormationType = formation;

	m_GameRoom->BroadcastPacket( &outPacket );
}

void Corps::CalculateHP()
{
	m_UnitNum = static_cast<int>( ( m_HP + 9 ) / 10 ); // hp가 1이라도 1명이 생존해 있을수 있게 하기위해 9를 더한다.
}


void Corps::DoNextAction( Action* addedAction, ULONGLONG remainTime )
{
	m_GameRoom->AddActionToScheduler( addedAction, remainTime );
}


float Corps::GetTargetLength( D3DXVECTOR2 destination )
{
	D3DXVECTOR2 vector;
	vector.x = destination.x - m_Position.m_EyePoint.x;
	vector.y = destination.y - m_Position.m_EyePoint.z;

	return D3DXVec2Length( &vector );
}


ULONGLONG Corps::MoveStart( D3DXVECTOR2 destination, int divideMove )
{
	MoveStop();

	m_IsMoving = true;

	float nowX = m_Position.m_EyePoint.x;
	float nowZ = m_Position.m_EyePoint.z;
	float targetX = destination.x;
	float targetZ = destination.y;

	D3DXVECTOR2 vector;
	vector.x = targetX - nowX;
	vector.y = targetZ - nowZ;

	float length = GetTargetLength( destination );

	length = length / divideMove;

	float speed = GetSpeed();
	D3DXVec2Normalize( &vector, &vector );

	ULONGLONG movingTime = static_cast<ULONGLONG>( ( length * 1000 ) / speed );
	m_MovingStartedTime = GetTickCount64();
	m_MovingDuringTime = movingTime;

	m_MovingRoute.m_EyePoint = { m_Position.m_EyePoint.x, 0.0f, m_Position.m_EyePoint.z };
	m_MovingRoute.m_LookAtPoint = { vector.x, 0.0f, vector.y };


	MoveCorpsResult outPacket;
	outPacket.m_CorpsID = m_CorpsID;
	outPacket.m_Speed = speed;
	outPacket.m_TargetX = targetX;
	outPacket.m_TargetZ = targetZ;
	outPacket.m_LookX = vector.x;
	outPacket.m_LookZ = vector.y;
	Log( "Move [%f][%f][%f][%f] \n", targetX, targetZ, vector.x, vector.y );
	m_GameRoom->BroadcastPacket( &outPacket );

	return movingTime;
}

void Corps::MoveStop()
{
	ReCalculatePosition();
	m_IsMoving = false;

	StopCorpsResult outPacket;
	outPacket.m_CorpsID = m_CorpsID;
	outPacket.m_NowX = m_Position.m_EyePoint.x;
	outPacket.m_NowZ = m_Position.m_EyePoint.z;
	outPacket.m_LookX = m_Position.m_LookAtPoint.x;
	outPacket.m_LookZ = m_Position.m_LookAtPoint.z;
	m_GameRoom->BroadcastPacket( &outPacket );
	Log( "Stop [%f][%f][%f][%f] \n", m_Position.m_EyePoint.x, m_Position.m_EyePoint.z, m_Position.m_LookAtPoint.x, m_Position.m_LookAtPoint.z );
}


void Corps::AttackCorps( Corps* targetCrops )
{
	targetCrops->MoveStop( );
	Action* targetAction = targetCrops->GetHoldingAction( );

	// targetCorps의 액션이 없으면(idle)이면 반격
	// 아니면 그냥 무시하고 계속 진행
	if ( !targetAction || ACTION_END == targetAction->GetActionStatus() )
	{
		Log( "target CounterAttack! \n" );
		// m_TargerCrops->ChangeFormation( FormationType::FORMATION_DEFENSE );// 망진으로 변경해야함
		Attack* action = new Attack();
		action->SetGameRoom( m_GameRoom );
		action->SetOwnerCorps( targetCrops );
		action->SetTargetCorps( this );

		m_GameRoom->AddActionToScheduler( action, targetCrops->GetAttackDelay( ) / 3 ); // 반격하려고 정신차리는 딜레이
	}


	// 공격 하세요
	// attack result packet 보내기
	targetCrops->AddDamage( GetAttackPower( ) );

	const PositionInfo& targetPositionInfo = targetCrops->GetPositionInfo( );

	AttackCorpsResult outPacket;
	outPacket.m_AttackerCorpsID = GetCorpsID();
	outPacket.m_TargetCorpsID = targetCrops->GetCorpsID( );
	outPacket.m_AttackerNowX = m_Position.m_EyePoint.x;
	outPacket.m_AttackerNowZ = m_Position.m_EyePoint.z;

	outPacket.m_AttackerLookX = targetPositionInfo.m_EyePoint.x;
	outPacket.m_AttackerLookZ = targetPositionInfo.m_EyePoint.z;

	outPacket.m_TargetNowX = targetPositionInfo.m_EyePoint.x;
	outPacket.m_TargetNowZ = targetPositionInfo.m_EyePoint.z;

	outPacket.m_TargetLookX = m_Position.m_EyePoint.x;
	outPacket.m_TargetLookZ = m_Position.m_EyePoint.z;

	outPacket.m_TargetUnitNum = targetCrops->GetUnitNum( );


	m_GameRoom->BroadcastPacket( &outPacket );

	Log( "[Attack] range:%f damage:%f \n", GetAttackRange(), GetAttackPower() );
	Log( "[Attack] Attacker:[%f][%f] Defencer:[%f][%f] \n", GetPositionInfo( ).m_EyePoint.x, GetPositionInfo( ).m_EyePoint.z, targetCrops->GetPositionInfo( ).m_EyePoint.x, targetCrops->GetPositionInfo( ).m_EyePoint.z );

}

void Corps::ReCalculatePosition()
{
	if ( m_IsMoving )
	{
		ULONGLONG elapsedTime = GetTickCount64() - m_MovingStartedTime;
		
		PositionInfo nowPosition;
		nowPosition.m_EyePoint = m_MovingRoute.m_EyePoint + ( m_MovingRoute.m_LookAtPoint *   GetSpeed()  * static_cast<float>( elapsedTime ) / 1000 );
		nowPosition.m_LookAtPoint = nowPosition.m_EyePoint + m_MovingRoute.m_LookAtPoint;
		SetPositionInfo( nowPosition );

	}
}



