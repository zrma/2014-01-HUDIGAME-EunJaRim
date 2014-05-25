#include "stdafx.h"
#include "Arrow.h"
#include "UnitStatus.h"

Arrow::Arrow( int playerID, int corpsID, UnitType unitType, PositionInfo position, GameRoom* gameRoom )
:Corps( playerID, corpsID, unitType, position, gameRoom )
{
	m_HP = g_Arrow.HP;
	m_MoveSpeed = g_Arrow.MoveSpeed;
	m_MoveSpeedFormationBonus = g_Arrow.MoveSpeedFormationBonus;
	m_MoveSpeedMapBonus = g_Arrow.MoveSpeedMapBonus;

	m_AttackRange = g_Arrow.AttackRange;
	m_AttackRangeFormationBonus = g_Arrow.AttackRangeFormationBonus;
	m_AttackRangeMapBonus = g_Arrow.AttackRangeMapBonus;

	m_AttackPower = g_Arrow.AttackPower;
	m_AttackPowerFormationBonus = g_Arrow.AttackPowerFormationBonus;
	m_AttackPowerMapBonus = g_Arrow.AttackPowerMapBonus;

	m_Defense = g_Arrow.Defense;
	m_DefenseFormationBonus = g_Arrow.DefenseFormationBonus;
	m_DefenseMapBonus = g_Arrow.DefenseMapBonus;

	m_AttackDelay = g_Arrow.AttackDelay;
	m_AttackDelayFormationBonus = g_Arrow.AttackDelayFormationBonus;
	m_AttackDelayMapBonus = g_Arrow.AttackDelayMapBonus;

	CalculateHP();
}

Arrow::~Arrow()
{
}


