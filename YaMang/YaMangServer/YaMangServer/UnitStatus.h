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
	// std::array<float, static_cast<size_t>(UnitType::UNIT_MAX)> UnitHP; // max 256 용량은 둘째치고 매번 캐스팅하면서 쓰느리 그냥 노가다?
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

	static float			g_GuardHP;
	static float			g_GuardMoveSpeed;
	static float			g_GuardMoveSpeedBonus;
	static float			g_GuardAttackRange;
	static float			g_GuardAttackRangeBonus;
	static float			g_GuardAttackPower;
	static float			g_GuardAttackPowerBonus;
	static float			g_GuardDefense;
	static float			g_GuardDefenseBonus;
	static ULONGLONG		g_GuardAttackDelay;
	static ULONGLONG		g_GuardAttackDelayBonus;

	static float			g_KnightHP;
	static float			g_KnightMoveSpeed;
	static float			g_KnightMoveSpeedBonus;
	static float			g_KnightAttackRange;
	static float			g_KnightAttackRangeBonus;
	static float			g_KnightAttackPower;
	static float			g_KnightAttackPowerBonus;
	static float			g_KnightDefense;
	static float			g_KnightDefenseBonus;
	static ULONGLONG		g_KnightAttackDelay;
	static ULONGLONG		g_KnightAttackDelayBonus;

	static float			g_PikeHP;
	static float			g_PikeMoveSpeed;
	static float			g_PikeMoveSpeedBonus;
	static float			g_PikeAttackRange;
	static float			g_PikeAttackRangeBonus;
	static float			g_PikeAttackPower;
	static float			g_PikeAttackPowerBonus;
	static float			g_PikeDefense;
	static float			g_PikeDefenseBonus;
	static ULONGLONG		g_PikeAttackDelay;
	static ULONGLONG		g_PikeAttackDelayBonus;

	static float			g_SwordHP;
	static float			g_SwordMoveSpeed;
	static float			g_SwordMoveSpeedBonus;
	static float			g_SwordAttackRange;
	static float			g_SwordAttackRangeBonus;
	static float			g_SwordAttackPower;
	static float			g_SwordAttackPowerBonus;
	static float			g_SwordDefense;
	static float			g_SwordDefenseBonus;
	static ULONGLONG		g_SwordAttackDelay;
	static ULONGLONG		g_SwordAttackDelayBonus;

};

