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
#include "CameraController.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "King.h"
#include "NetworkManager.h"
#include "MapManager.h"
#include "SceneManager.h"
#include "Scene.h"
#include "ScenePlay.h"
#include "Frustum.h"

Corps::Corps( int corpsId, int playerId, PositionInfo pos )
: m_CorpsID( corpsId ), m_OwnerPlayerID( playerId )
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

	if ( m_OwnerPlayerID == NetworkManager::GetInstance()->GetMyPlayerID() )
	{
		m_MeshKey = MESH_KEY_CORPS_RUSH_MINE;
	}
	else
	{
		m_MeshKey = MESH_KEY_CORPS_RUSH_ENEMY;
	}
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
			case  UnitType::UNIT_KING:
				unit = new King( this, i );
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
	if ( m_UnitList.empty() )
	{
		SetVisible( false );
		return;
	}

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

	if ( !m_Visible || !m_MeshKey || CameraController::GetInstance()->GetHeightGrade() <= 3 )
	{
		return;
	}

	if ( SCENE_PLAY == SceneManager::GetInstance()->GetNowSceneType() )
	{
		Scene* scene = SceneManager::GetInstance()->GetNowScene();
		ScenePlay* scenePlay = static_cast<ScenePlay*>( scene );

		Frustum* frustum = scenePlay->GetFrustum();

		if ( frustum )
		{
			D3DXVECTOR3	center = m_EyePoint;
			if ( !frustum->IsIn( &center ) )
			{
				return;
			}
		}
	}

	D3DXMATRIXA16 thisMatrix = GetMatrix();

	float hp = static_cast<float>( m_UnitList.size() * 0.008 );

	if ( IsSelected() )
	{
		D3DXMATRIXA16 scaleMatrix;
		D3DXMatrixScaling( &scaleMatrix, 0.6f + hp, 0.6f + hp, 0.6f + hp );

		thisMatrix = scaleMatrix * thisMatrix;
	}
	else
	{
		D3DXMATRIXA16 scaleMatrix;
		D3DXMatrixScaling( &scaleMatrix, 0.5f + hp, 0.5f + hp, 0.5f + hp );

		thisMatrix = scaleMatrix * thisMatrix;
	}

	D3DXMATRIXA16 heightMatrix;
	D3DXMatrixTranslation( &heightMatrix, 0, MapManager::GetInstance()->GetHeightByPosition( m_EyePoint.x, m_EyePoint.z ), 0 );
	thisMatrix = thisMatrix * heightMatrix;
	Renderer::GetInstance()->SetWorldMatrix( thisMatrix );

	ResourceMesh* mesh = ResourceManager::GetInstance()->GetMeshByKey( m_MeshKey );

	if ( mesh )
	{
		Renderer::GetInstance()->RenderMesh( mesh->m_MeshObject );
	}
}

void Corps::SetVisible( bool visible )
{
	m_Visible = visible;

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
	InterpolatePosition();

	D3DXVECTOR3 view = m_LookAtPoint - m_EyePoint;

	float time = static_cast<float>( Timer::GetInstance()->GetElapsedTime() ) / 1000;
	float delta = time * m_Speed / ( m_IsInterpolated ? 2 : 1 );

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
	std::shared_ptr<Collision> clickCollision(new Collision( this, 3.0f + CameraController::GetInstance()->GetHeightGrade() ));
	
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

void Corps::SetFormation( FormationType formation )
{
	m_TargetFormation = formation;

	switch ( formation )
	{
		case FormationType::FORMATION_DEFENSE:
		{
			if ( m_OwnerPlayerID == NetworkManager::GetInstance()->GetMyPlayerID() )
			{
				m_MeshKey = MESH_KEY_CORPS_DEFENSE_MINE;
			}
			else
			{
				m_MeshKey = MESH_KEY_CORPS_DEFENSE_ENEMY;
			}
		}
			break;
		case FormationType::FORMATION_DESTROY:
		{
			if ( m_OwnerPlayerID == NetworkManager::GetInstance()->GetMyPlayerID() )
			{
				m_MeshKey = MESH_KEY_CORPS_DESTROY_MINE;
			}
			else
			{
				m_MeshKey = MESH_KEY_CORPS_DESTROY_ENEMY;
			}
		}
			break;
		case FormationType::FORMATION_RUSH:
		case FormationType::FORMATION_NONE:
		case FormationType::FORMATION_MAX:
		default:
		{
			if ( m_OwnerPlayerID == NetworkManager::GetInstance()->GetMyPlayerID() )
			{
				m_MeshKey = MESH_KEY_CORPS_RUSH_MINE;
			}
			else
			{
				m_MeshKey = MESH_KEY_CORPS_RUSH_ENEMY;
			}
		}
			break;
	}
}

void Corps::InterpolatePosition()
{
	D3DXVECTOR3 distance = m_TargetPoint - m_EyePoint;
	if ( D3DXVec3Length( &distance ) > 3.0f )
	{
		float time = static_cast<float>( Timer::GetInstance()->GetElapsedTime() ) / 1000;
		float delta = time * m_Speed / ( m_IsMoved ? 2 : 1 );

		// Log( "목표치까지 거리 %4f ( %4f %4f ) -> (%4f %4f ) \n", D3DXVec3Length( &distance ),
		// 	 m_EyePoint.x, m_EyePoint.z, m_TargetPoint.x, m_TargetPoint.z);

		D3DXVec3Normalize( &distance, &distance );
		m_EyePoint += delta * distance;
		m_LookAtPoint += delta * distance;

		m_IsInterpolated = true;
	}
	else
	{
		m_IsInterpolated = false;
	}
}
