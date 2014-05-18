#include "stdafx.h"
#include "Arrow.h"
#include "UnitStatus.h"

Arrow::Arrow( int playerID, int corpsID, UnitType unitType, PositionInfo position, GameRoom* clientManager )
:Corps( playerID, corpsID, unitType, position, clientManager )
{
	m_HP = UnitStatus::g_Arrow.HP;
	m_MoveSpeed = UnitStatus::g_Arrow.MoveSpeed;
	m_MoveSpeedBonus = UnitStatus::g_Arrow.MoveSpeedBonus;

	m_AttackRange = UnitStatus::g_Arrow.AttackRange;
	m_AttackRangeBonus = UnitStatus::g_Arrow.AttackRangeBonus;

	m_AttackPower = UnitStatus::g_Arrow.AttackPower;
	m_AttackPowerBonus = UnitStatus::g_Arrow.AttackPowerBonus;

	m_Defense = UnitStatus::g_Arrow.Defense;
	m_DefenseBonus = UnitStatus::g_Arrow.DefenseBonus;

	m_AttackDelay = UnitStatus::g_Arrow.AttackDelay;
	m_AttackDelayBonus = UnitStatus::g_Arrow.AttackDelayBonus;

	CalculateHP();
}

Arrow::~Arrow()
{
}


