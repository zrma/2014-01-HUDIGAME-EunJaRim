#include "stdafx.h"
#include "UnitStatus.h"
#include "tinyxml.h"
#include "xpath_static.h"
#include "MacroSet.h"

UnitData UnitStatus::g_Arrow;
UnitData UnitStatus::g_Guard;
UnitData UnitStatus::g_Knight;
UnitData UnitStatus::g_Pike;
UnitData UnitStatus::g_Sword;
UnitData UnitStatus::g_King;

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
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Arrow/MoveSpeedBonus/text()" ).c_str();
		g_Arrow.MoveSpeedBonus = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Arrow/AttackRange/text()" ).c_str();
		g_Arrow.AttackRange = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Arrow/AttackRangeBonus/text()" ).c_str();
		g_Arrow.AttackRangeBonus = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Arrow/AttackPower/text()" ).c_str();
		g_Arrow.AttackPower = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Arrow/AttackPowerBonus/text()" ).c_str();
		g_Arrow.AttackPowerBonus = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Arrow/Defense/text()" ).c_str();
		g_Arrow.Defense = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Arrow/DefenseBonus/text()" ).c_str();
		g_Arrow.DefenseBonus = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Arrow/AttackDelay/text()" ).c_str();
		g_Arrow.AttackDelay = std::stoull( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Arrow/AttackDelayBonus/text()" ).c_str();
		g_Arrow.AttackDelayBonus = std::stoull( data );


		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Guard/HP/text()" ).c_str();
		g_Guard.HP = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Guard/MoveSpeed/text()" ).c_str();
		g_Guard.MoveSpeed = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Guard/MoveSpeedBonus/text()" ).c_str();
		g_Guard.MoveSpeedBonus = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Guard/AttackRange/text()" ).c_str();
		g_Guard.AttackRange = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Guard/AttackRangeBonus/text()" ).c_str();
		g_Guard.AttackRangeBonus = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Guard/AttackPower/text()" ).c_str();
		g_Guard.AttackPower = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Guard/AttackPowerBonus/text()" ).c_str();
		g_Guard.AttackPowerBonus = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Guard/Defense/text()" ).c_str();
		g_Guard.Defense = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Guard/DefenseBonus/text()" ).c_str();
		g_Guard.DefenseBonus = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Guard/AttackDelay/text()" ).c_str();
		g_Guard.AttackDelay = std::stoull( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Guard/AttackDelayBonus/text()" ).c_str();
		g_Guard.AttackDelayBonus = std::stoull( data );


		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Knight/HP/text()" ).c_str();
		g_Knight.HP = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Knight/MoveSpeed/text()" ).c_str();
		g_Knight.MoveSpeed = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Knight/MoveSpeedBonus/text()" ).c_str();
		g_Knight.MoveSpeedBonus = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Knight/AttackRange/text()" ).c_str();
		g_Knight.AttackRange = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Knight/AttackRangeBonus/text()" ).c_str();
		g_Knight.AttackRangeBonus = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Knight/AttackPower/text()" ).c_str();
		g_Knight.AttackPower = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Knight/AttackPowerBonus/text()" ).c_str();
		g_Knight.AttackPowerBonus = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Knight/Defense/text()" ).c_str();
		g_Knight.Defense = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Knight/DefenseBonus/text()" ).c_str();
		g_Knight.DefenseBonus = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Knight/AttackDelay/text()" ).c_str();
		g_Knight.AttackDelay = std::stoull( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Knight/AttackDelayBonus/text()" ).c_str();
		g_Knight.AttackDelayBonus = std::stoull( data );


		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Pike/HP/text()" ).c_str();
		g_Pike.HP = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Pike/MoveSpeed/text()" ).c_str();
		g_Pike.MoveSpeed = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Pike/MoveSpeedBonus/text()" ).c_str();
		g_Pike.MoveSpeedBonus = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Pike/AttackRange/text()" ).c_str();
		g_Pike.AttackRange = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Pike/AttackRangeBonus/text()" ).c_str();
		g_Pike.AttackRangeBonus = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Pike/AttackPower/text()" ).c_str();
		g_Pike.AttackPower = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Pike/AttackPowerBonus/text()" ).c_str();
		g_Pike.AttackPowerBonus = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Pike/Defense/text()" ).c_str();
		g_Pike.Defense = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Pike/DefenseBonus/text()" ).c_str();
		g_Pike.DefenseBonus = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Pike/AttackDelay/text()" ).c_str();
		g_Pike.AttackDelay = std::stoull( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Pike/AttackDelayBonus/text()" ).c_str();
		g_Pike.AttackDelayBonus = std::stoull( data );


		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Sword/HP/text()" ).c_str();
		g_Sword.HP = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Sword/MoveSpeed/text()" ).c_str();
		g_Sword.MoveSpeed = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Sword/MoveSpeedBonus/text()" ).c_str();
		g_Sword.MoveSpeedBonus = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Sword/AttackRange/text()" ).c_str();
		g_Sword.AttackRange = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Sword/AttackRangeBonus/text()" ).c_str();
		g_Sword.AttackRangeBonus = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Sword/AttackPower/text()" ).c_str();
		g_Sword.AttackPower = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Sword/AttackPowerBonus/text()" ).c_str();
		g_Sword.AttackPowerBonus = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Sword/Defense/text()" ).c_str();
		g_Sword.Defense = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Sword/DefenseBonus/text()" ).c_str();
		g_Sword.DefenseBonus = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Sword/AttackDelay/text()" ).c_str();
		g_Sword.AttackDelay = std::stoull( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/Sword/AttackDelayBonus/text()" ).c_str();
		g_Sword.AttackDelayBonus = std::stoull( data );



		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/King/HP/text()" ).c_str();
		g_King.HP = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/King/MoveSpeed/text()" ).c_str();
		g_King.MoveSpeed = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/King/MoveSpeedBonus/text()" ).c_str();
		g_King.MoveSpeedBonus = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/King/AttackRange/text()" ).c_str();
		g_King.AttackRange = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/King/AttackRangeBonus/text()" ).c_str();
		g_King.AttackRangeBonus = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/King/AttackPower/text()" ).c_str();
		g_King.AttackPower = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/King/AttackPowerBonus/text()" ).c_str();
		g_King.AttackPowerBonus = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/King/Defense/text()" ).c_str();
		g_King.Defense = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/King/DefenseBonus/text()" ).c_str();
		g_King.DefenseBonus = std::stof( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/King/AttackDelay/text()" ).c_str();
		g_King.AttackDelay = std::stoull( data );
		data = TinyXPath::S_xpath_string( document.RootElement(), "/Unit/King/AttackDelayBonus/text()" ).c_str();
		g_King.AttackDelayBonus = std::stoull( data );

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
