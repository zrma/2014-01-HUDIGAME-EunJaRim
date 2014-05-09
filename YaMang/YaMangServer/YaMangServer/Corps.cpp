#include "stdafx.h"
#include "Corps.h"
#include "Arrow.h"
#include "Guard.h"
#include "Sword.h"
#include "Pike.h"
#include "knight.h"
#include "EnumSet.h"
#include "PacketType.h"
#include "ClientManager.h"

Corps::Corps( int playerID, int corpsID, UnitType unitType, PositionInfo position, ClientManager* clientManager )
: m_PlayerID( playerID ), m_CorpsID( corpsID ), m_UnitType(unitType), m_Position( position ), m_ClientManager( clientManager )
{
}


Corps::~Corps()
{
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
			}
		}
		
	}

	float targetHP = m_HP + 9; // hp가 1이라도 1명이 생존해 있을수 있게 하기위해 9를 더한다.
	m_UnitNum = static_cast<int>( targetHP / 10 );
}

void Corps::DoNextAction( Action* addedAction, ULONGLONG remainTime )
{
	m_ClientManager->AddActionToScheduler( addedAction, remainTime );
}

void Corps::MoveStart( ULONGLONG movingDuringTime, D3DXVECTOR2 lookVector )
{
	m_IsMoving = true;

	m_MovingStartedTime = GetTickCount64();
	m_MovingDuringTime = movingDuringTime;

	m_MovingRoute.m_EyePoint = { m_Position.m_EyePoint.x, 0.0f, m_Position.m_EyePoint.z };
	m_MovingRoute.m_LookAtPoint = { lookVector.x, 0.0f, lookVector.y };
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
	m_ClientManager->BroadcastPacket( &outPacket );

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


