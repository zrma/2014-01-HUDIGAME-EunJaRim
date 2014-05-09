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

bool Corps::IsContain( float x, float z ) const
{
	D3DXMATRIXA16 thisMatrix = GetMatrix();

	D3DXVECTOR3 minBoundary = m_FormationArray[static_cast<size_t>( m_TargetFormation )]->m_Min;
	D3DXVECTOR3 maxBoundary = m_FormationArray[static_cast<size_t>( m_TargetFormation )]->m_Max;

	D3DXVECTOR3 leftTop = { minBoundary.x, 0, minBoundary.z };
	D3DXVECTOR3 rightTop = { maxBoundary.z, 0, minBoundary.z };
	D3DXVECTOR3 leftBottom = { minBoundary.x, 0, maxBoundary.z };
	D3DXVECTOR3 rightBottom = { maxBoundary.x, 0, maxBoundary.z };
	
	D3DXVec3TransformCoord( &leftTop, &leftTop, &thisMatrix );
	D3DXVec3TransformCoord( &rightTop, &rightTop, &thisMatrix );
	D3DXVec3TransformCoord( &leftBottom, &leftBottom, &thisMatrix );
	D3DXVec3TransformCoord( &rightBottom, &rightBottom, &thisMatrix );
	
	/*Log( "%f %f, %f %f, %f %f, %f %f 사각박스!",
		 leftTop.x, leftTop.z, rightTop.x, rightTop.z,
		 leftBottom.x, leftBottom.z, rightBottom.x, rightBottom.z );*/

	if ( ( ( leftTop.x - x > 0 ) && ( rightTop.x - x > 0 ) && ( leftBottom.x - x > 0 ) && ( rightBottom.x - x > 0 ) ) ||
		 ( ( leftTop.x - x < 0 ) && ( rightTop.x - x < 0 ) && ( leftBottom.x - x < 0 ) && ( rightBottom.x - x < 0 ) ) ||
		 ( ( leftTop.z - z > 0 ) && ( rightTop.z - z > 0 ) && ( leftBottom.z - z > 0 ) && ( rightBottom.z - z > 0 ) ) ||
		 ( ( leftTop.z - z < 0 ) && ( rightTop.z - z < 0 ) && ( leftBottom.z - z < 0 ) && ( rightBottom.z - z < 0 ) ) )
	{
		return false;
	}

	return true;
}
