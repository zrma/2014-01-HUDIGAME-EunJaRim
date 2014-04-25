#include "stdafx.h"
#include "BreadBoard.h"
#include "Arrow.h"
#include "Guard.h"
#include "Knight.h"
#include "Pike.h"
#include "Sword.h"
#include "MacroSet.h"
#include "Corps.h"
#include "MacroSet.h"
#include "Action.h"

Corps::Corps( int corpsId, int playerId, Position pos )
: m_CorpsId( corpsId ), m_OwnerPlayerID( playerId ), m_TargetFormation( FormationType::FORMATION_RUSH )
{
	m_FormationArray.fill( nullptr );
	m_FormationArray[static_cast<size_t>( FormationType::FORMATION_DEFENSE )] = new DefenseBread();
	m_FormationArray[static_cast<size_t>( FormationType::FORMATION_DESTROY )] = new DestroyBread();
	m_FormationArray[static_cast<size_t>( FormationType::FORMATION_RUSH )] = new RushBread();

	m_EyePoint = pos.m_EyePoint;
	m_LookAtPoint = pos.m_LookAtPoint;
	m_UpVector = { 0.0f, 1.0f, 0.0f };
	m_Scale = { 1.0f, 1.0f, 1.0f };

	ActionDefault action;
	m_Action = action;
}

Corps::~Corps()
{
	for ( auto& toBeDelete : m_UnitList )
	{
		SafeDelete( toBeDelete );
	}

	for ( auto& toBeDelete : m_FormationArray )
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
			case  UnitType::UNIT_ARROW:
				unit = new Arrow( this, i );
				break;
			case  UnitType::UNIT_GUARD:
				unit = new Guard( this, i );
				break;
			case  UnitType::UNIT_KNIGHT:
				unit = new Knight( this, i );
				break;
			case  UnitType::UNIT_PIKE:
				unit = new Pike( this, i );
				break;
			case  UnitType::UNIT_SWORD:
				unit = new Sword( this, i );
				break;
			default:
				return;
		}

		if ( unit )
		{
			m_UnitList.push_back( unit );
			unit = nullptr;
		}
	}
}

void Corps::Update()
{
	DoAction();
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

D3DXVECTOR3 Corps::GetFormation( int unitId ) const
{
	return m_FormationArray[static_cast<size_t>(m_TargetFormation)]->m_Position[unitId];
}

void Corps::GoFoward()
{
	D3DXVECTOR3 view = m_LookAtPoint - m_EyePoint;

	m_EyePoint += m_TargetVector;
	m_LookAtPoint += m_TargetVector;
}

void Corps::ClearAction()
{
	ActionDefault act; m_Action = act;
}

void Corps::SetAct( ActFunc act )
{
	for ( auto& iter : m_UnitList )
	{
		iter->ChangeAct( act );
	}
}
