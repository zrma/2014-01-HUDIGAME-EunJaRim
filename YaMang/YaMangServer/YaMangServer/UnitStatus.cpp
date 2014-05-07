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
