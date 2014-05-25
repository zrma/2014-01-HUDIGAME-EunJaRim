#include "stdafx.h"
#include "Pike.h"
#include "UnitStatus.h"

Pike::Pike( int playerID, int corpsID, UnitType unitType, PositionInfo position, GameRoom* gameRoom )
:Corps( playerID, corpsID, unitType, position, gameRoom )
{
	m_HP = UnitStatus::g_Pike.HP;
	m_MoveSpeed = UnitStatus::g_Pike.MoveSpeed;
	m_MoveSpeedBonus = UnitStatus::g_Pike.MoveSpeedBonus;

	m_AttackRange = UnitStatus::g_Pike.AttackRange;
	m_AttackRangeBonus = UnitStatus::g_Pike.AttackRangeBonus;

	m_AttackPower = UnitStatus::g_Pike.AttackPower;
	m_AttackPowerBonus = UnitStatus::g_Pike.AttackPowerBonus;

	m_Defense = UnitStatus::g_Pike.Defense;
	m_DefenseBonus = UnitStatus::g_Pike.DefenseBonus;

	m_AttackDelay = UnitStatus::g_Pike.AttackDelay;
	m_AttackDelayBonus = UnitStatus::g_Pike.AttackDelayBonus;

	CalculateHP();
}

Pike::~Pike()
{
}
