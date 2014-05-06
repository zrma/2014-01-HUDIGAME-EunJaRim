#include "stdafx.h"
#include "Guard.h"

Guard::Guard( int playerID, int corpsID, PositionInfo position, ActionScheduler* actionScheduler ):Corps( playerID, corpsID, position, actionScheduler )
{
	// xml로 뺄것
	m_HP = 100.0f;
	m_MoveSpeed = 3.0f;
	m_AttackRange = 1.0f;
	m_AttackPower = 1.0f;
	m_Defense = 10.0f;
	m_AttackDelay = 5000;
}

Guard::~Guard()
{
}