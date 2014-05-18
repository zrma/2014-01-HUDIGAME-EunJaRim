#include "stdafx.h"
#include "Sword.h"
#include "UnitStatus.h"

Sword::Sword( int playerID, int corpsID, UnitType unitType, PositionInfo position, GameRoom* clientManager )
:Corps( playerID, corpsID, unitType, position, clientManager )
{
	m_HP = UnitStatus::g_Sword.HP;
	m_MoveSpeed = UnitStatus::g_Sword.MoveSpeed;
	m_MoveSpeedBonus = UnitStatus::g_Sword.MoveSpeedBonus;

	m_AttackRange = UnitStatus::g_Sword.AttackRange;
	m_AttackRangeBonus = UnitStatus::g_Sword.AttackRangeBonus;

	m_AttackPower = UnitStatus::g_Sword.AttackPower;
	m_AttackPowerBonus = UnitStatus::g_Sword.AttackPowerBonus;

	m_Defense = UnitStatus::g_Sword.Defense;
	m_DefenseBonus = UnitStatus::g_Sword.DefenseBonus;

	m_AttackDelay = UnitStatus::g_Sword.AttackDelay;
	m_AttackDelayBonus = UnitStatus::g_Sword.AttackDelayBonus;

	CalculateHP();
}

Sword::~Sword()
{
}