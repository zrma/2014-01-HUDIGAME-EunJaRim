#include "stdafx.h"
#include "Sword.h"
#include "UnitStatus.h"

Sword::Sword( int playerID, int corpsID, UnitType unitType, PositionInfo position, GameRoom* gameRoom )
:Corps( playerID, corpsID, unitType, position, gameRoom )
{
	m_HP = g_Sword.HP;
	m_MoveSpeed = g_Sword.MoveSpeed;
	m_MoveSpeedFormationBonus = g_Sword.MoveSpeedFormationBonus;
	m_MoveSpeedMapBonus = g_Sword.MoveSpeedMapBonus;

	m_AttackRange = g_Sword.AttackRange;
	m_AttackRangeFormationBonus = g_Sword.AttackRangeFormationBonus;
	m_AttackRangeMapBonus = g_Sword.AttackRangeMapBonus;

	m_AttackPower = g_Sword.AttackPower;
	m_AttackPowerFormationBonus = g_Sword.AttackPowerFormationBonus;
	m_AttackPowerMapBonus = g_Sword.AttackPowerMapBonus;

	m_Defense = g_Sword.Defense;
	m_DefenseFormationBonus = g_Sword.DefenseFormationBonus;
	m_DefenseMapBonus = g_Sword.DefenseMapBonus;

	m_AttackDelay = g_Sword.AttackDelay;
	m_AttackDelayFormationBonus = g_Sword.AttackDelayFormationBonus;
	m_AttackDelayMapBonus = g_Sword.AttackDelayMapBonus;

	CalculateHP();
}

Sword::~Sword()
{
}