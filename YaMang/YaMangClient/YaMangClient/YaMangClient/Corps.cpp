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
#include "Timer.h"
#include "Collision.h"
#include "CollisionManager.h"

Corps::Corps( int corpsId, int playerId, PositionInfo pos )
: m_CorpsID( corpsId ), m_OwnerPlayerID( playerId ), m_TargetFormation( FormationType::FORMATION_RUSH )
{
	m_FormationArray.fill( nullptr );
	m_FormationArray[static_cast<size_t>( FormationType::FORMATION_DEFENSE )] = new DefenseBread();
	m_FormationArray[static_cast<size_t>( FormationType::FORMATION_DESTROY )] = new DestroyBread();
	m_FormationArray[static_cast<size_t>( FormationType::FORMATION_RUSH )] = new RushBread();

	m_EyePoint = pos.m_EyePoint;
	m_TargetPoint = pos.m_EyePoint;
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
	m_UnitList.clear();

	for ( auto& toBeDelete : m_FormationArray )
	{
		SafeDelete( toBeDelete );
	}
	m_FormationArray.fill( nullptr );
}

void Corps::Create( UINT num, UnitType unitType )
{
	m_UnitType = unitType;

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

void Corps::Render() const
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

	float time = static_cast<float>( Timer::GetInstance()->GetElapsedTime() ) / 1000;
	float delta = time * m_Speed;

	m_EyePoint += view * delta;
	m_LookAtPoint += view * delta;
}

void Corps::ClearAction()
{
	ActionDefault act;
	m_Action = act;
}

void Corps::SetAct( ActFunc act )
{
	for ( auto& iter : m_UnitList )
	{
		iter->ChangeAct( act );
	}
}

void Corps::SetActStatus( ActionStatusType actionStatus )
{
	for ( auto& iter : m_UnitList )
	{
		iter->SetActionStatus( actionStatus );
	}
}

void Corps::LookForward()
{
	for ( auto& iter : m_UnitList )
	{
		iter->LookForward();
	}
}

void Corps::SetCorpsHP( int unitNum )
{
	int nowUnitNum = m_UnitList.size();

	while ( nowUnitNum > unitNum )
	{
		if ( nowUnitNum < 1)
		{
			return;
		}
		Unit* unit = m_UnitList.back( );

		// unit->죽는모션?
		// 좀비(?) 매니저 한테 넘겨야 됨
		// 임시로 제거
		SafeDelete( unit );
		
		m_UnitList.pop_back();
		nowUnitNum = m_UnitList.size( );
	}
}

bool Corps::IsContain( float x, float z )
{
	std::shared_ptr<Collision> clickCollision(new Collision( this, 3.0f ));
	
	D3DXVECTOR3 backupPosition = m_EyePoint;
	m_EyePoint.x = x;
	m_EyePoint.z = z;

	CollisionManager::GetInstance()->CheckCollision( &(*clickCollision) );
	m_EyePoint = backupPosition;

	if ( clickCollision->IsCollide() )
	{
		Unit* unit = dynamic_cast<Unit*>( clickCollision->GetCompetitor() );
		if ( unit && unit->GetOwner()->GetCorpsID() == m_CorpsID )
		{
			return true;
		}
	}

	return false;
}
