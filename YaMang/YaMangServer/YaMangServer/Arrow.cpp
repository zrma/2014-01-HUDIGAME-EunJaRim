#include "stdafx.h"
#include "Arrow.h"

Arrow::Arrow( int playerID, int corpsID, PositionInfo position, ActionScheduler* actionScheduler ):Corps( playerID, corpsID, position, actionScheduler )
{
	// xml로 뺄것
	m_HP = 100.0f;
	m_MoveSpeed = 3.0f;
	m_AttackRange = 12.f;
	m_AttackPower = 15.0f;
	m_Defense = 1.0f;
	m_AttackDelay = 5000;
}

Arrow::~Arrow()
{
}


