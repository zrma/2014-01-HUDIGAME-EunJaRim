#include "stdafx.h"
#include "Corps.h"
#include "Arrow.h"
#include "Guard.h"
#include "Knight.h"
#include "Pike.h"
#include "Sword.h"
#include "MacroSet.h"

Corps::Corps()
{
}


Corps::~Corps()
{
	for ( auto& toBeDelete : m_UnitList )
	{
		SafeDelete( toBeDelete );
	}
}

void Corps::Create( UINT num, UnitType unitType )
{
	m_UnitList.reserve( num );
	Unit* unit = nullptr;

	for ( UINT i = 0; i < num; ++i )
	{
		switch ( unitType )
		{
			case UNIT_ARROW:
				unit = new Arrow();
				m_UnitList.push_back( unit );
				break;
			case UNIT_GUARD:
				unit = new Guard();
				m_UnitList.push_back( unit );
				break;
			case UNIT_KNIGHT:
				unit = new Knight();
				m_UnitList.push_back( unit );
				break;
			case UNIT_PIKE:
				unit = new Pike();
				m_UnitList.push_back( unit );
				break;
			case UNIT_SWORD:
				unit = new Sword();
				m_UnitList.push_back( unit );
				break;
			default:
				return;
		}
	}
}

void Corps::Update()
{
	for ( auto& iter : m_UnitList )
	{
		iter->Update();
	}
}

void Corps::Render()
{
	for ( auto& iter : m_UnitList )
	{
		iter->Render();
	}
}

void Corps::SetVisible( bool visible )
{
	for ( auto& iter : m_UnitList )
	{
		iter->SetVisible( visible );
	}
}
