#include "stdafx.h"
#include "Guard.h"
#include "UnitStatus.h"

Guard::Guard( int playerID, int corpsID, UnitType unitType, PositionInfo position, GameRoom* gameRoom )
:Corps( playerID, corpsID, unitType, position, gameRoom )
{
	m_HP = g_Guard.HP;
	m_MoveSpeed = g_Guard.MoveSpeed;
	m_MoveSpeedFormationBonus = g_Guard.MoveSpeedFormationBonus;
	m_MoveSpeedMapBonus = g_Guard.MoveSpeedMapBonus;

	m_AttackRange = g_Guard.AttackRange;
	m_AttackRangeFormationBonus = g_Guard.AttackRangeFormationBonus;
	m_AttackRangeMapBonus = g_Guard.AttackRangeMapBonus;

	m_AttackPower = g_Guard.AttackPower;
	m_AttackPowerFormationBonus = g_Guard.AttackPowerFormationBonus;
	m_AttackPowerMapBonus = g_Guard.AttackPowerMapBonus;

	m_Defense = g_Guard.Defense;
	m_DefenseFormationBonus = g_Guard.DefenseFormationBonus;
	m_DefenseMapBonus = g_Guard.DefenseMapBonus;

	m_AttackDelay = g_Guard.AttackDelay;
	m_AttackDelayFormationBonus = g_Guard.AttackDelayFormationBonus;
	m_AttackDelayMapBonus = g_Guard.AttackDelayMapBonus;

	CalculateHP();
}

Guard::~Guard()
{
}