#include "stdafx.h"
#include "Pike.h"
#include "UnitStatus.h"

Pike::Pike( int playerID, int corpsID, UnitType unitType, PositionInfo position, GameRoom* gameRoom )
:Corps( playerID, corpsID, unitType, position, gameRoom )
{
	m_HP = g_Pike.HP;
	m_MoveSpeed = g_Pike.MoveSpeed;
	m_MoveSpeedFormationBonus = g_Pike.MoveSpeedFormationBonus;
	m_MoveSpeedMapBonus = g_Pike.MoveSpeedMapBonus;

	m_AttackRange = g_Pike.AttackRange;
	m_AttackRangeFormationBonus = g_Pike.AttackRangeFormationBonus;
	m_AttackRangeMapBonus = g_Pike.AttackRangeMapBonus;

	m_AttackPower = g_Pike.AttackPower;
	m_AttackPowerFormationBonus = g_Pike.AttackPowerFormationBonus;
	m_AttackPowerMapBonus = g_Pike.AttackPowerMapBonus;

	m_Defense = g_Pike.Defense;
	m_DefenseFormationBonus = g_Pike.DefenseFormationBonus;
	m_DefenseMapBonus = g_Pike.DefenseMapBonus;

	m_AttackDelay = g_Pike.AttackDelay;
	m_AttackDelayFormationBonus = g_Pike.AttackDelayFormationBonus;
	m_AttackDelayMapBonus = g_Pike.AttackDelayMapBonus;

	CalculateHP();
}

Pike::~Pike()
{
}
