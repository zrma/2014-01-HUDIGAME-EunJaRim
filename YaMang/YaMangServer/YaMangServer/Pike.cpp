#include "stdafx.h"
#include "Pike.h"
#include "UnitStatus.h"

Pike::Pike( int playerID, int corpsID, UnitType unitType, PositionInfo position, GameRoom* clientManager )
:Corps( playerID, corpsID, unitType, position, clientManager )
{
	m_HP = UnitStatus::g_PikeHP;
	m_MoveSpeed = UnitStatus::g_PikeMoveSpeed;
	m_MoveSpeedBonus = UnitStatus::g_PikeMoveSpeedBonus;

	m_AttackRange = UnitStatus::g_PikeAttackRange;
	m_AttackRangeBonus = UnitStatus::g_PikeAttackRangeBonus;

	m_AttackPower = UnitStatus::g_PikeAttackPower;
	m_AttackPowerBonus = UnitStatus::g_PikeAttackPowerBonus;

	m_Defense = UnitStatus::g_PikeDefense;
	m_DefenseBonus = UnitStatus::g_PikeDefenseBonus;

	m_AttackDelay = UnitStatus::g_PikeAttackDelay;
	m_AttackDelayBonus = UnitStatus::g_PikeAttackDelayBonus;

	CalculateHP();
}

Pike::~Pike()
{
}
