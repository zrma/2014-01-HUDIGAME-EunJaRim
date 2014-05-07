#include "stdafx.h"
#include "Knight.h"
#include "UnitStatus.h"

Knight::Knight( int playerID, int corpsID, PositionInfo position, ActionScheduler* actionScheduler ):Corps( playerID, corpsID, position, actionScheduler )
{
	m_HP = UnitStatus::g_KnightHP;
	m_MoveSpeed = UnitStatus::g_KnightMoveSpeed;
	m_MoveSpeedBonus = UnitStatus::g_KnightMoveSpeedBonus;

	m_AttackRange = UnitStatus::g_KnightAttackRange;
	m_AttackRangeBonus = UnitStatus::g_KnightAttackRangeBonus;

	m_AttackPower = UnitStatus::g_KnightAttackPower;
	m_AttackPowerBonus = UnitStatus::g_KnightAttackPowerBonus;

	m_Defense = UnitStatus::g_KnightDefense;
	m_DefenseBonus = UnitStatus::g_KnightDefenseBonus;

	m_AttackDelay = UnitStatus::g_KnightAttackDelay;
	m_AttackDelayBonus = UnitStatus::g_KnightAttackDelayBonus;
}

Knight::~Knight()
{
}
