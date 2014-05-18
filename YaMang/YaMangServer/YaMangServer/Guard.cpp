#include "stdafx.h"
#include "Guard.h"
#include "UnitStatus.h"

Guard::Guard( int playerID, int corpsID, UnitType unitType, PositionInfo position, GameRoom* clientManager )
:Corps( playerID, corpsID, unitType, position, clientManager )
{
	m_HP = UnitStatus::g_Guard.HP;
	m_MoveSpeed = UnitStatus::g_Guard.MoveSpeed;
	m_MoveSpeedBonus = UnitStatus::g_Guard.MoveSpeedBonus;

	m_AttackRange = UnitStatus::g_Guard.AttackRange;
	m_AttackRangeBonus = UnitStatus::g_Guard.AttackRangeBonus;

	m_AttackPower = UnitStatus::g_Guard.AttackPower;
	m_AttackPowerBonus = UnitStatus::g_Guard.AttackPowerBonus;

	m_Defense = UnitStatus::g_Guard.Defense;
	m_DefenseBonus = UnitStatus::g_Guard.DefenseBonus;

	m_AttackDelay = UnitStatus::g_Guard.AttackDelay;
	m_AttackDelayBonus = UnitStatus::g_Guard.AttackDelayBonus;

	CalculateHP();
}

Guard::~Guard()
{
}