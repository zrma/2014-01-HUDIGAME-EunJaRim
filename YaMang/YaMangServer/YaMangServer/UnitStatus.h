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
	static UnitData	g_Arrow; ///# 이것들 static으로 일일이 놓지 말고 어차피 read-only데이터니까.. 그냥 싱글톤이나 전역 객체로 처리... 
	static UnitData	g_Guard; ///# 멤버 네이밍도 다른 m_ 처럼 통일... g_xxxx 는 말그대로 레알 전역 변수인 경우만.
	static UnitData	g_Knight;
	static UnitData	g_Pike;
	static UnitData	g_Sword;
	static UnitData	g_King;

};

