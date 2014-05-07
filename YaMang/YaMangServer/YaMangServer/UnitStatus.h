#pragma once
#include "EnumSet.h"

class UnitStatus
{
public:
	static bool LoadUnitStatus( );

private:
	UnitStatus();
	~UnitStatus();

public:
	// std::array<float, static_cast<size_t>(UnitType::UNIT_MAX)> UnitHP;
	static float			g_ArrowHP;
	static float			g_ArrowMoveSpeed;
	static float			g_ArrowMoveSpeedBonus;
	static float			g_ArrowAttackRange;
	static float			g_ArrowAttackRangeBonus;
	static float			g_ArrowAttackPower;
	static float			g_ArrowAttackPowerBonus;
	static float			g_ArrowDefense;
	static float			g_ArrowDefenseBonus;
	static ULONGLONG		g_ArrowAttackDelay;
	static ULONGLONG		g_ArrowAttackDelayBonus;

};

