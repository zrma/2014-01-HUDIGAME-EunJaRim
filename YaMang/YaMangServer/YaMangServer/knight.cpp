#include "stdafx.h"
#include "Knight.h"
#include "UnitStatus.h"

Knight::Knight( int playerID, int corpsID, UnitType unitType, PositionInfo position, GameRoom* gameRoom )
:Corps( playerID, corpsID, unitType, position, gameRoom )
{
	m_HP = g_Knight.HP;
	m_MoveSpeed = g_Knight.MoveSpeed;
	m_MoveSpeedFormationBonus = g_Knight.MoveSpeedFormationBonus;
	m_MoveSpeedMapBonus = g_Knight.MoveSpeedMapBonus;

	m_AttackRange = g_Knight.AttackRange;
	m_AttackRangeFormationBonus = g_Knight.AttackRangeFormationBonus;
	m_AttackRangeMapBonus = g_Knight.AttackRangeMapBonus;

	m_AttackPower = g_Knight.AttackPower;
	m_AttackPowerFormationBonus = g_Knight.AttackPowerFormationBonus;
	m_AttackPowerMapBonus = g_Knight.AttackPowerMapBonus;

	m_Defense = g_Knight.Defense;
	m_DefenseFormationBonus = g_Knight.DefenseFormationBonus;
	m_DefenseMapBonus = g_Knight.DefenseMapBonus;

	m_AttackDelay = g_Knight.AttackDelay;
	m_AttackDelayFormationBonus = g_Knight.AttackDelayFormationBonus;
	m_AttackDelayMapBonus = g_Knight.AttackDelayMapBonus;

	CalculateHP();
}

Knight::~Knight()
{
}
