#include "stdafx.h"
#include "Sword.h"
#include "UnitStatus.h"

Sword::Sword( int playerID, int corpsID, PositionInfo position, ActionScheduler* actionScheduler ):Corps( playerID, corpsID, position, actionScheduler )
{
	m_HP = UnitStatus::g_SwordHP;
	m_MoveSpeed = UnitStatus::g_SwordMoveSpeed;
	m_MoveSpeedBonus = UnitStatus::g_SwordMoveSpeedBonus;

	m_AttackRange = UnitStatus::g_SwordAttackRange;
	m_AttackRangeBonus = UnitStatus::g_SwordAttackRangeBonus;

	m_AttackPower = UnitStatus::g_SwordAttackPower;
	m_AttackPowerBonus = UnitStatus::g_SwordAttackPowerBonus;

	m_Defense = UnitStatus::g_SwordDefense;
	m_DefenseBonus = UnitStatus::g_SwordDefenseBonus;

	m_AttackDelay = UnitStatus::g_SwordAttackDelay;
	m_AttackDelayBonus = UnitStatus::g_SwordAttackDelayBonus;
}

Sword::~Sword()
{
}