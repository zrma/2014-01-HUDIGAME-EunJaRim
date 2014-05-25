#include "stdafx.h"
#include "UnitStatus.h"
#include "tinyxml.h"
#include "xpath_static.h"
#include "MacroSet.h"

UnitData g_Arrow;
UnitData g_Guard;
UnitData g_Knight;
UnitData g_Pike;
UnitData g_Sword;
UnitData g_King;

bool UnitStatus::LoadUnitStatus()
{
	TiXmlDocument document = TiXmlDocument( "./Config/UnitStatus.xml" );
	bool m_LoadSuccess = document.LoadFile();

	if ( m_LoadSuccess )
	{
		std::string data;
		
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Arrow/HP/text()" ).c_str();
		g_Arrow.HP = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Arrow/MoveSpeed/text()" ).c_str();
		g_Arrow.MoveSpeed = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Arrow/MoveSpeedFormationBonus/text()" ).c_str();
		g_Arrow.MoveSpeedFormationBonus = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Arrow/MoveSpeedMapBonus/text()" ).c_str();
		g_Arrow.MoveSpeedMapBonus = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Arrow/AttackRange/text()" ).c_str();
		g_Arrow.AttackRange = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Arrow/AttackRangeFormationBonus/text()" ).c_str();
		g_Arrow.AttackRangeFormationBonus = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Arrow/AttackRangeMapBonus/text()" ).c_str();
		g_Arrow.AttackRangeMapBonus = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Arrow/AttackPower/text()" ).c_str();
		g_Arrow.AttackPower = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Arrow/AttackPowerFormationBonus/text()" ).c_str();
		g_Arrow.AttackPowerFormationBonus = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Arrow/AttackPowerMapBonus/text()" ).c_str();
		g_Arrow.AttackPowerMapBonus = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Arrow/Defense/text()" ).c_str();
		g_Arrow.Defense = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Arrow/DefenseFormationBonus/text()" ).c_str();
		g_Arrow.DefenseFormationBonus = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Arrow/DefenseMapBonus/text()" ).c_str();
		g_Arrow.DefenseMapBonus = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Arrow/AttackDelay/text()" ).c_str();
		g_Arrow.AttackDelay = std::stoull( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Arrow/AttackDelayFormationBonus/text()" ).c_str();
		g_Arrow.AttackDelayFormationBonus = std::stoull( data );


		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Guard/HP/text()" ).c_str();
		g_Guard.HP = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Guard/MoveSpeed/text()" ).c_str();
		g_Guard.MoveSpeed = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Guard/MoveSpeedFormationBonus/text()" ).c_str();
		g_Guard.MoveSpeedFormationBonus = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Guard/MoveSpeedMapBonus/text()" ).c_str();
		g_Guard.MoveSpeedMapBonus = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Guard/AttackRange/text()" ).c_str();
		g_Guard.AttackRange = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Guard/AttackRangeFormationBonus/text()" ).c_str();
		g_Guard.AttackRangeFormationBonus = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Guard/AttackRangeMapBonus/text()" ).c_str();
		g_Guard.AttackRangeMapBonus = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Guard/AttackPower/text()" ).c_str();
		g_Guard.AttackPower = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Guard/AttackPowerFormationBonus/text()" ).c_str();
		g_Guard.AttackPowerFormationBonus = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Guard/AttackPowerMapBonus/text()" ).c_str();
		g_Guard.AttackPowerMapBonus = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Guard/Defense/text()" ).c_str();
		g_Guard.Defense = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Guard/DefenseFormationBonus/text()" ).c_str();
		g_Guard.DefenseFormationBonus = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Guard/DefenseMapBonus/text()" ).c_str();
		g_Guard.DefenseMapBonus = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Guard/AttackDelay/text()" ).c_str();
		g_Guard.AttackDelay = std::stoull( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Guard/AttackDelayFormationBonus/text()" ).c_str();
		g_Guard.AttackDelayFormationBonus = std::stoull( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Guard/AttackDelayMapBonus/text()" ).c_str();
		g_Guard.AttackDelayMapBonus = std::stoull( data );


		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Knight/HP/text()" ).c_str();
		g_Knight.HP = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Knight/MoveSpeed/text()" ).c_str();
		g_Knight.MoveSpeed = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Knight/MoveSpeedFormationBonus/text()" ).c_str();
		g_Knight.MoveSpeedFormationBonus = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Knight/MoveSpeedMapBonus/text()" ).c_str();
		g_Knight.AttackRangeMapBonus = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Knight/AttackRange/text()" ).c_str();
		g_Knight.AttackRange = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Knight/AttackRangeFormationBonus/text()" ).c_str();
		g_Knight.AttackRangeFormationBonus = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Knight/AttackRangeMapBonus/text()" ).c_str();
		g_Knight.AttackRangeFormationBonus = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Knight/AttackPower/text()" ).c_str();
		g_Knight.AttackPower = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Knight/AttackPowerFormationBonus/text()" ).c_str();
		g_Knight.AttackPowerFormationBonus = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Knight/AttackPowerMapBonus/text()" ).c_str();
		g_Knight.AttackPowerMapBonus = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Knight/Defense/text()" ).c_str();
		g_Knight.Defense = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Knight/DefenseFormationBonus/text()" ).c_str();
		g_Knight.DefenseFormationBonus = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Knight/DefenseMapBonus/text()" ).c_str();
		g_Knight.DefenseMapBonus = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Knight/AttackDelay/text()" ).c_str();
		g_Knight.AttackDelay = std::stoull( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Knight/AttackDelayFormationBonus/text()" ).c_str();
		g_Knight.AttackDelayFormationBonus = std::stoull( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Knight/AttackDelayMapBonus/text()" ).c_str();
		g_Knight.AttackDelayMapBonus = std::stoull( data );


		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Pike/HP/text()" ).c_str();
		g_Pike.HP = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Pike/MoveSpeed/text()" ).c_str();
		g_Pike.MoveSpeed = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Pike/MoveSpeedFormationBonus/text()" ).c_str();
		g_Pike.MoveSpeedFormationBonus = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Pike/MoveSpeedMapBonus/text()" ).c_str();
		g_Pike.MoveSpeedMapBonus = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Pike/AttackRange/text()" ).c_str();
		g_Pike.AttackRange = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Pike/AttackRangeFormationBonus/text()" ).c_str();
		g_Pike.AttackRangeFormationBonus = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Pike/AttackRangeMapBonus/text()" ).c_str();
		g_Pike.AttackRangeMapBonus = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Pike/AttackPower/text()" ).c_str();
		g_Pike.AttackPower = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Pike/AttackPowerFormationBonus/text()" ).c_str();
		g_Pike.AttackPowerFormationBonus = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Pike/AttackPowerMapBonus/text()" ).c_str();
		g_Pike.AttackPowerMapBonus = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Pike/Defense/text()" ).c_str();
		g_Pike.Defense = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Pike/DefenseFormationBonus/text()" ).c_str();
		g_Pike.DefenseFormationBonus = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Pike/DefenseMapBonus/text()" ).c_str();
		g_Pike.DefenseMapBonus = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Pike/AttackDelay/text()" ).c_str();
		g_Pike.AttackDelay = std::stoull( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Pike/AttackDelayFormationBonus/text()" ).c_str();
		g_Pike.AttackDelayFormationBonus = std::stoull( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Pike/AttackDelayMapBonus/text()" ).c_str();
		g_Pike.AttackDelayMapBonus = std::stoull( data );


		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Sword/HP/text()" ).c_str();
		g_Sword.HP = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Sword/MoveSpeed/text()" ).c_str();
		g_Sword.MoveSpeed = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Sword/MoveSpeedFormationBonus/text()" ).c_str();
		g_Sword.MoveSpeedFormationBonus = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Sword/MoveSpeedMapBonus/text()" ).c_str();
		g_Sword.MoveSpeedMapBonus = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Sword/AttackRange/text()" ).c_str();
		g_Sword.AttackRange = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Sword/AttackRangeFormationBonus/text()" ).c_str();
		g_Sword.AttackRangeFormationBonus = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Sword/AttackRangeMapBonus/text()" ).c_str();
		g_Sword.AttackRangeMapBonus = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Sword/AttackPower/text()" ).c_str();
		g_Sword.AttackPower = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Sword/AttackPowerFormationBonus/text()" ).c_str();
		g_Sword.AttackPowerFormationBonus = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Sword/AttackPowerMapBonus/text()" ).c_str();
		g_Sword.AttackPowerMapBonus = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Sword/Defense/text()" ).c_str();
		g_Sword.Defense = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Sword/DefenseFormationBonus/text()" ).c_str();
		g_Sword.DefenseFormationBonus = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Sword/DefenseMapBonus/text()" ).c_str();
		g_Sword.DefenseMapBonus = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Sword/AttackDelay/text()" ).c_str();
		g_Sword.AttackDelay = std::stoull( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Sword/AttackDelayFormationBonus/text()" ).c_str();
		g_Sword.AttackDelayFormationBonus = std::stoull( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Sword/AttackDelayMapBonus/text()" ).c_str();
		g_Sword.AttackDelayMapBonus = std::stoull( data );


		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/King/HP/text()" ).c_str();
		g_King.HP = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/King/MoveSpeed/text()" ).c_str();
		g_King.MoveSpeed = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/King/MoveSpeedFormationBonus/text()" ).c_str();
		g_King.MoveSpeedFormationBonus = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/King/MoveSpeedMapBonus/text()" ).c_str();
		g_King.MoveSpeedMapBonus = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/King/AttackRange/text()" ).c_str();
		g_King.AttackRange = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/King/AttackRangeFormationBonus/text()" ).c_str();
		g_King.AttackRangeFormationBonus = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/King/AttackRangeMapBonus/text()" ).c_str();
		g_King.AttackRangeMapBonus = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/King/AttackPower/text()" ).c_str();
		g_King.AttackPower = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/King/AttackPowerFormationBonus/text()" ).c_str();
		g_King.AttackPowerFormationBonus = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/King/AttackPowerMapBonus/text()" ).c_str();
		g_King.AttackPowerMapBonus = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/King/Defense/text()" ).c_str();
		g_King.Defense = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/King/DefenseFormationBonus/text()" ).c_str();
		g_King.DefenseFormationBonus = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/King/DefenseMapBonus/text()" ).c_str();
		g_King.DefenseMapBonus = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/King/AttackDelay/text()" ).c_str();
		g_King.AttackDelay = std::stoull( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/King/AttackDelayFormationBonus/text()" ).c_str();
		g_King.AttackDelayFormationBonus = std::stoull( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/King/AttackDelayMapBonus/text()" ).c_str();
		g_King.AttackDelayMapBonus = std::stoull( data );


		Log( "UnitStatus Load OK! \n" );

		return true;
	}
	else
	{
		Log( "UnitStatus Load Fail! \n" );
	}

	return false;
}

UnitStatus::UnitStatus()
{
}


UnitStatus::~UnitStatus()
{
}
