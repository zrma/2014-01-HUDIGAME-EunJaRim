#include "stdafx.h"
#include "Corps.h"
#include "Arrow.h"
#include "Guard.h"
#include "Sword.h"
#include "Pike.h"
#include "knight.h"
#include "EnumSet.h"

Corps::Corps()
{
	m_Position.posX = NOT_A_NUMBER_FLOAT;
	m_Position.posZ = NOT_A_NUMBER_FLOAT;
}


Corps::~Corps()
{
}

void Corps::GenerateCorps( UnitType type, int corpsID, Position position )
{

	m_CorpsId = corpsID;
	m_Position = position;

	m_UnitList.clear();
	m_UnitList.reserve( 10 );
	switch ( type )
	{
		case UNIT_ARROW:
			for ( int i = 0; i < 10; ++i )
			{
				m_UnitList.push_back( new Arrow() );
			}
			break;
		case UNIT_GUARD:
			// 가드의 인원수?
			for ( int i = 0; i < 5; ++i )
			{
				m_UnitList.push_back( new Guard() );
			}
			break;
		case UNIT_KNIGHT:
			for ( int i = 0; i < 10; ++i )
			{
				m_UnitList.push_back( new Knight() );
			}
			break;
		case UNIT_PIKE:
			for ( int i = 0; i < 10; ++i )
			{
				m_UnitList.push_back( new Pike() );
			}
			break;
		case UNIT_SWORD:
			for ( int i = 0; i < 10; ++i )
			{
				m_UnitList.push_back( new Sword() );
			}
			break;
		default:
			return;
	}
}
