#include "stdafx.h"
#include "Arrow.h"
#include "UnitStatus.h"

Arrow::Arrow( int playerID, int corpsID, UnitType unitType, PositionInfo position, ClientManager* clientManager )
:Corps( playerID, corpsID, unitType, position, clientManager )
{
	m_HP = UnitStatus::g_ArrowHP;
	m_MoveSpeed = UnitStatus::g_ArrowMoveSpeed;
	m_MoveSpeedBonus = UnitStatus::g_ArrowMoveSpeedBonus;

	m_AttackRange = UnitStatus::g_ArrowAttackRange;
	m_AttackRangeBonus = UnitStatus::g_ArrowAttackRangeBonus;

	m_AttackPower = UnitStatus::g_ArrowAttackPower;
	m_AttackPowerBonus = UnitStatus::g_ArrowAttackPowerBonus;

	m_Defense = UnitStatus::g_ArrowDefense;
	m_DefenseBonus = UnitStatus::g_ArrowDefenseBonus;

	m_AttackDelay = UnitStatus::g_ArrowAttackDelay;
	m_AttackDelayBonus = UnitStatus::g_ArrowAttackDelayBonus;

	CalculateHP();
}

Arrow::~Arrow()
{
}


