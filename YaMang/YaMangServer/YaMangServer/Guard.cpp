#include "stdafx.h"
#include "Guard.h"
#include "UnitStatus.h"

Guard::Guard( int playerID, int corpsID, UnitType unitType, PositionInfo position, GameRoom* clientManager )
:Corps( playerID, corpsID, unitType, position, clientManager )
{
	m_HP = UnitStatus::g_GuardHP;
	m_MoveSpeed = UnitStatus::g_GuardMoveSpeed;
	m_MoveSpeedBonus = UnitStatus::g_GuardMoveSpeedBonus;

	m_AttackRange = UnitStatus::g_GuardAttackRange;
	m_AttackRangeBonus = UnitStatus::g_GuardAttackRangeBonus;

	m_AttackPower = UnitStatus::g_GuardAttackPower;
	m_AttackPowerBonus = UnitStatus::g_GuardAttackPowerBonus;

	m_Defense = UnitStatus::g_GuardDefense;
	m_DefenseBonus = UnitStatus::g_GuardDefenseBonus;

	m_AttackDelay = UnitStatus::g_GuardAttackDelay;
	m_AttackDelayBonus = UnitStatus::g_GuardAttackDelayBonus;

	CalculateHP();
}

Guard::~Guard()
{
}