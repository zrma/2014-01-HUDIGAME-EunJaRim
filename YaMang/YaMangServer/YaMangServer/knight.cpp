#include "stdafx.h"
#include "Knight.h"


Knight::Knight( int playerID, int corpsID, PositionInfo position, ActionScheduler* actionScheduler ):Corps( playerID, corpsID, position, actionScheduler )
{
	// xml·Î »¬°Í
	m_HP = 100.0f;
	m_MoveSpeed = 12.0f;
	m_AttackRange = 3.0f;
	m_AttackPower = 15.0f;
	m_Defense = 1.0f;
	m_AttackDelay = 3000;
}

Knight::~Knight()
{
}
