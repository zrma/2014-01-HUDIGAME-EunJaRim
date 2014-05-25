#include "stdafx.h"
#include "King.h"
#include "UnitStatus.h"

King::King( int playerID, int corpsID, UnitType unitType, PositionInfo position, GameRoom* gameRoom )
:Corps( playerID, corpsID, unitType, position, gameRoom )
{
	m_HP = UnitStatus::g_King.HP;
	m_MoveSpeed = UnitStatus::g_King.MoveSpeed;
	m_MoveSpeedBonus = UnitStatus::g_King.MoveSpeedBonus;

	m_AttackRange = UnitStatus::g_King.AttackRange;
	m_AttackRangeBonus = UnitStatus::g_King.AttackRangeBonus;

	m_AttackPower = UnitStatus::g_King.AttackPower;
	m_AttackPowerBonus = UnitStatus::g_King.AttackPowerBonus;

	m_Defense = UnitStatus::g_King.Defense;
	m_DefenseBonus = UnitStatus::g_King.DefenseBonus;

	m_AttackDelay = UnitStatus::g_King.AttackDelay;
	m_AttackDelayBonus = UnitStatus::g_King.AttackDelayBonus;

	CalculateHP();
}


King::~King()
{
}
