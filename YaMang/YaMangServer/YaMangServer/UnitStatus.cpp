#include "stdafx.h"
#include "UnitStatus.h"
#include "tinyxml.h"
#include "xpath_static.h"

float UnitStatus::g_ArrowHP = 50.0f;
float UnitStatus::g_ArrowMoveSpeed = 3.0f;
float UnitStatus::g_ArrowMoveSpeedBonus = 0.0f;
float UnitStatus::g_ArrowAttackRange = 1.0f;
float UnitStatus::g_ArrowAttackRangeBonus = 0.0f;
float UnitStatus::g_ArrowAttackPower = 15.0f;
float UnitStatus::g_ArrowAttackPowerBonus = 0.0f;
float UnitStatus::g_ArrowDefense = 3.0f;
float UnitStatus::g_ArrowDefenseBonus = 0.0f;
ULONGLONG UnitStatus::g_ArrowAttackDelay = 3000;
ULONGLONG UnitStatus::g_ArrowAttackDelayBonus = 0;

float UnitStatus::g_GuardHP = 100.0f;
float UnitStatus::g_GuardMoveSpeed = 3.0f;
float UnitStatus::g_GuardMoveSpeedBonus = 0.0f;
float UnitStatus::g_GuardAttackRange = 1.0f;
float UnitStatus::g_GuardAttackRangeBonus = 0.0f;
float UnitStatus::g_GuardAttackPower = 10.0f;
float UnitStatus::g_GuardAttackPowerBonus = 0.0f;
float UnitStatus::g_GuardDefense = 6.0f;
float UnitStatus::g_GuardDefenseBonus = 0.0f;
ULONGLONG UnitStatus::g_GuardAttackDelay = 5000;
ULONGLONG UnitStatus::g_GuardAttackDelayBonus = 0;

float UnitStatus::g_KnightHP = 100.0f;
float UnitStatus::g_KnightMoveSpeed = 12.0f;
float UnitStatus::g_KnightMoveSpeedBonus = 0.0f;
float UnitStatus::g_KnightAttackRange = 3.0f;
float UnitStatus::g_KnightAttackRangeBonus = 0.0f;
float UnitStatus::g_KnightAttackPower = 15.0f;
float UnitStatus::g_KnightAttackPowerBonus = 0.0f;
float UnitStatus::g_KnightDefense = 1.0f;
float UnitStatus::g_KnightDefenseBonus = 0.0f;
ULONGLONG UnitStatus::g_KnightAttackDelay = 3000;
ULONGLONG UnitStatus::g_KnightAttackDelayBonus = 0;

float UnitStatus::g_PikeHP = 100.0f;
float UnitStatus::g_PikeMoveSpeed = 3.0f;
float UnitStatus::g_PikeMoveSpeedBonus = 0.0f;
float UnitStatus::g_PikeAttackRange = 3.0f;
float UnitStatus::g_PikeAttackRangeBonus = 0.0f;
float UnitStatus::g_PikeAttackPower = 12.0f;
float UnitStatus::g_PikeAttackPowerBonus = 0.0f;
float UnitStatus::g_PikeDefense = 5.0f;
float UnitStatus::g_PikeDefenseBonus = 0.0f;
ULONGLONG UnitStatus::g_PikeAttackDelay = 2000;
ULONGLONG UnitStatus::g_PikeAttackDelayBonus = 0;

float UnitStatus::g_SwordHP = 100.0f;
float UnitStatus::g_SwordMoveSpeed = 3.0f;
float UnitStatus::g_SwordMoveSpeedBonus = 0.0f;
float UnitStatus::g_SwordAttackRange = 3.0f;
float UnitStatus::g_SwordAttackRangeBonus = 0.0f;
float UnitStatus::g_SwordAttackPower = 8.0f;
float UnitStatus::g_SwordAttackPowerBonus = 0.0f;
float UnitStatus::g_SwordDefense = 5.0f;
float UnitStatus::g_SwordDefenseBonus = 0.0f;
ULONGLONG UnitStatus::g_SwordAttackDelay = 1000;
ULONGLONG UnitStatus::g_SwordAttackDelayBonus = 0;


bool UnitStatus::LoadUnitStatus()
{
	TiXmlDocument document = TiXmlDocument( "UnitStatus.xml" );
	bool m_LoadSuccess = document.LoadFile();

	if ( m_LoadSuccess )
	{
		std::string data;
		
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Arrow/HP/text()" ).c_str();
		g_ArrowHP = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Arrow/MoveSpeed/text()" ).c_str();
		g_ArrowMoveSpeed = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Arrow/MoveSpeedBonus/text()" ).c_str();
		g_ArrowMoveSpeedBonus = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Arrow/AttackRange/text()" ).c_str();
		g_ArrowAttackRange = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Arrow/AttackRangeBonus/text()" ).c_str();
		g_ArrowAttackRangeBonus = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Arrow/AttackPower/text()" ).c_str();
		g_ArrowAttackPower = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Arrow/AttackPowerBonus/text()" ).c_str();
		g_ArrowAttackPowerBonus = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Arrow/Defense/text()" ).c_str();
		g_ArrowDefense = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Arrow/DefenseBonus/text()" ).c_str();
		g_ArrowDefenseBonus = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Arrow/AttackDelay/text()" ).c_str();
		g_ArrowAttackDelay = std::stoull( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Arrow/AttackDelayBonus/text()" ).c_str();
		g_ArrowAttackDelayBonus = std::stoull( data );


		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Guard/HP/text()" ).c_str();
		g_GuardHP = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Guard/MoveSpeed/text()" ).c_str();
		g_GuardMoveSpeed = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Guard/MoveSpeedBonus/text()" ).c_str();
		g_GuardMoveSpeedBonus = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Guard/AttackRange/text()" ).c_str();
		g_GuardAttackRange = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Guard/AttackRangeBonus/text()" ).c_str();
		g_GuardAttackRangeBonus = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Guard/AttackPower/text()" ).c_str();
		g_GuardAttackPower = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Guard/AttackPowerBonus/text()" ).c_str();
		g_GuardAttackPowerBonus = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Guard/Defense/text()" ).c_str();
		g_GuardDefense = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Guard/DefenseBonus/text()" ).c_str();
		g_GuardDefenseBonus = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Guard/AttackDelay/text()" ).c_str();
		g_GuardAttackDelay = std::stoull( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Guard/AttackDelayBonus/text()" ).c_str();
		g_GuardAttackDelayBonus = std::stoull( data );


		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Knight/HP/text()" ).c_str();
		g_KnightHP = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Knight/MoveSpeed/text()" ).c_str();
		g_KnightMoveSpeed = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Knight/MoveSpeedBonus/text()" ).c_str();
		g_KnightMoveSpeedBonus = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Knight/AttackRange/text()" ).c_str();
		g_KnightAttackRange = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Knight/AttackRangeBonus/text()" ).c_str();
		g_KnightAttackRangeBonus = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Knight/AttackPower/text()" ).c_str();
		g_KnightAttackPower = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Knight/AttackPowerBonus/text()" ).c_str();
		g_KnightAttackPowerBonus = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Knight/Defense/text()" ).c_str();
		g_KnightDefense = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Knight/DefenseBonus/text()" ).c_str();
		g_KnightDefenseBonus = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Knight/AttackDelay/text()" ).c_str();
		g_KnightAttackDelay = std::stoull( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Knight/AttackDelayBonus/text()" ).c_str();
		g_KnightAttackDelayBonus = std::stoull( data );


		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Pike/HP/text()" ).c_str();
		g_PikeHP = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Pike/MoveSpeed/text()" ).c_str();
		g_PikeMoveSpeed = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Pike/MoveSpeedBonus/text()" ).c_str();
		g_PikeMoveSpeedBonus = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Pike/AttackRange/text()" ).c_str();
		g_PikeAttackRange = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Pike/AttackRangeBonus/text()" ).c_str();
		g_PikeAttackRangeBonus = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Pike/AttackPower/text()" ).c_str();
		g_PikeAttackPower = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Pike/AttackPowerBonus/text()" ).c_str();
		g_PikeAttackPowerBonus = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Pike/Defense/text()" ).c_str();
		g_PikeDefense = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Pike/DefenseBonus/text()" ).c_str();
		g_PikeDefenseBonus = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Pike/AttackDelay/text()" ).c_str();
		g_PikeAttackDelay = std::stoull( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Pike/AttackDelayBonus/text()" ).c_str();
		g_PikeAttackDelayBonus = std::stoull( data );


		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Sword/HP/text()" ).c_str();
		g_SwordHP = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Sword/MoveSpeed/text()" ).c_str();
		g_SwordMoveSpeed = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Sword/MoveSpeedBonus/text()" ).c_str();
		g_SwordMoveSpeedBonus = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Sword/AttackRange/text()" ).c_str();
		g_SwordAttackRange = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Sword/AttackRangeBonus/text()" ).c_str();
		g_SwordAttackRangeBonus = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Sword/AttackPower/text()" ).c_str();
		g_SwordAttackPower = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Sword/AttackPowerBonus/text()" ).c_str();
		g_SwordAttackPowerBonus = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Sword/Defense/text()" ).c_str();
		g_SwordDefense = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Sword/DefenseBonus/text()" ).c_str();
		g_SwordDefenseBonus = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Sword/AttackDelay/text()" ).c_str();
		g_SwordAttackDelay = std::stoull( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Sword/AttackDelayBonus/text()" ).c_str();
		g_SwordAttackDelayBonus = std::stoull( data );

		printf_s( "UnitStatus Load OK! \n" );

		return true;
	}
	else
	{
		printf_s( "UnitStatus Load Fail! \n" );
	}

	return false;
}

UnitStatus::UnitStatus()
{
}


UnitStatus::~UnitStatus()
{
}
