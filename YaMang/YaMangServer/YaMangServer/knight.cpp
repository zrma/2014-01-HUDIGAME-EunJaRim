#include "stdafx.h"
#include "Knight.h"
#include "UnitStatus.h"

Knight::Knight( int playerID, int corpsID, UnitType unitType, PositionInfo position, GameRoom* clientManager )
:Corps( playerID, corpsID, unitType, position, clientManager )
{
	m_HP = UnitStatus::g_Knight.HP;
	m_MoveSpeed = UnitStatus::g_Knight.MoveSpeed;
	m_MoveSpeedBonus = UnitStatus::g_Knight.MoveSpeedBonus;

	m_AttackRange = UnitStatus::g_Knight.AttackRange;
	m_AttackRangeBonus = UnitStatus::g_Knight.AttackRangeBonus;

	m_AttackPower = UnitStatus::g_Knight.AttackPower;
	m_AttackPowerBonus = UnitStatus::g_Knight.AttackPowerBonus;

	m_Defense = UnitStatus::g_Knight.Defense;
	m_DefenseBonus = UnitStatus::g_Knight.DefenseBonus;

	m_AttackDelay = UnitStatus::g_Knight.AttackDelay;
	m_AttackDelayBonus = UnitStatus::g_Knight.AttackDelayBonus;

	CalculateHP();
}

Knight::~Knight()
{
}
