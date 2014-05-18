#pragma once
#include "EnumSet.h"

struct UnitData
{
	float		HP;
	float		MoveSpeed;
	float		MoveSpeedBonus;
	float		AttackRange;
	float		AttackRangeBonus;
	float		AttackPower;
	float		AttackPowerBonus;
	float		Defense;
	float		DefenseBonus;
	ULONGLONG	AttackDelay;
	ULONGLONG	AttackDelayBonus;
};

class UnitStatus
{
public:
	static bool LoadUnitStatus( );

private:
	UnitStatus();
	~UnitStatus();

public:
	// std::array<float, static_cast<size_t>(UnitType::UNIT_MAX)> UnitHP; // max 256 용량은 둘째치고 매번 캐스팅하면서 쓰느리 그냥 노가다?
	static UnitData	g_Arrow;
	static UnitData	g_Guard;
	static UnitData	g_Knight;
	static UnitData	g_Pike;
	static UnitData	g_Sword;
	static UnitData	g_King;

};

