#include "stdafx.h"
#include "King.h"
#include "UnitStatus.h"

King::King( int playerID, int corpsID, UnitType unitType, PositionInfo position, GameRoom* gameRoom )
:Corps( playerID, corpsID, unitType, position, gameRoom )
{
	m_HP = g_King.HP;
	m_MoveSpeed = g_King.MoveSpeed;
	m_MoveSpeedFormationBonus = g_King.MoveSpeedFormationBonus;
	m_MoveSpeedMapBonus = g_King.MoveSpeedMapBonus;

	m_AttackRange = g_King.AttackRange;
	m_AttackRangeFormationBonus = g_King.AttackRangeFormationBonus;
	m_AttackRangeMapBonus = g_King.AttackRangeMapBonus;

	m_AttackPower = g_King.AttackPower;
	m_AttackPowerFormationBonus = g_King.AttackPowerFormationBonus;
	m_AttackPowerMapBonus = g_King.AttackPowerMapBonus;

	m_Defense = g_King.Defense;
	m_DefenseFormationBonus = g_King.DefenseFormationBonus;
	m_DefenseMapBonus = g_King.DefenseMapBonus;

	m_AttackDelay = g_King.AttackDelay;
	m_AttackDelayFormationBonus = g_King.AttackDelayFormationBonus;
	m_AttackDelayMapBonus = g_King.AttackDelayMapBonus;

	CalculateHP();
}


King::~King()
{
}
