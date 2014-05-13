#include "stdafx.h"
#include "Corps.h"
#include "Unit.h"
#include "SharedDefine.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "Timer.h"
#include "Act.h"
#include "Collision.h"
#include "CollisionManager.h"
#include "CameraController.h"

Unit::Unit( Corps* owner, UINT unitId )
: m_Corps( owner ), m_UnitID( unitId )
{
	ClearAct();
	m_Collision = new Collision( this, 0.7f );
	CollisionManager::GetInstance()->AddCollision( m_Collision );
}

Unit::~Unit()
{
	CollisionManager::GetInstance()->RemoveColiision( m_Collision );
	SafeDelete( m_Collision );
}

void Unit::Update()
{
	DoAct();
}

void Unit::Render() const
{
	if ( !m_Visible || !m_MeshKey || CameraController::GetInstance()->GetHeightGrade() > 3 )
	{
		return;
	}

	D3DXMATRIXA16 thisMatrix = GetMatrix( false );

	if ( m_Corps->IsSelected() )
	{
		D3DXMATRIXA16 scaleMatrix;
		D3DXMatrixScaling( &scaleMatrix, 1.2f, 1.2f, 1.2f );

		thisMatrix = scaleMatrix * thisMatrix;
	}
	Renderer::GetInstance()->SetWorldMatrix( thisMatrix );

	ResourceMesh* mesh = ResourceManager::GetInstance()->GetMeshByKey( m_MeshKey );

	if ( mesh )
	{
		Renderer::GetInstance()->RenderMesh( mesh->m_MeshObject );
	}
}

void Unit::SetStartPosition()
{
	D3DXMATRIXA16 parentMatrix = m_Corps->GetMatrix();

	D3DXMATRIXA16 formMatrix;
	D3DXMatrixIdentity( &formMatrix );

	D3DXVECTOR3 formationVector = ( m_Corps->GetFormation( m_UnitID ) ) * 3;
	D3DXMatrixTranslation( &formMatrix, formationVector.x, formationVector.y, formationVector.z );

	formMatrix = formMatrix * parentMatrix;

	D3DXVec3TransformCoord( &m_EyePoint, &m_EyePoint, &formMatrix );
	D3DXVec3TransformCoord( &m_LookAtPoint, &m_LookAtPoint, &formMatrix );

	m_TargetPoint = m_LookAtPoint;
}

void Unit::FindDestination()
{
	D3DXMATRIXA16 parentMatrix = m_Corps->GetMatrix();

	D3DXMATRIXA16 formMatrix;
	D3DXMatrixIdentity( &formMatrix );

	D3DXVECTOR3 formationVector = ( m_Corps->GetFormation( m_UnitID ) ) * 3;
	D3DXMatrixTranslation( &formMatrix, formationVector.x, formationVector.y, formationVector.z );

	formMatrix = formMatrix * parentMatrix;

	D3DXVECTOR3	targetPoint = { 0.0f, 0.0f, 0.1f };
	D3DXVec3TransformCoord( &m_TargetPoint, &targetPoint, &formMatrix );

	/*Log( "%f %f %f \n", m_TargetPoint.x, m_TargetPoint.y, m_TargetPoint.z );
	Log( "%f %f %f \n", m_EyePoint.x, m_EyePoint.y, m_EyePoint.z );
	Log( "%f %f %f \n\n", m_LookAtPoint.x, m_LookAtPoint.y, m_LookAtPoint.z );*/
}

void Unit::RotateToDestination()
{
	m_ActionStatus = ACTION_STATUS_NONE;

	if ( m_LookAtPoint.x != m_TargetPoint.x )
	{
		m_LookAtPoint.x = m_TargetPoint.x;
		m_ActionStatus = ACTION_STATUS_ROTATE;
	}
	if ( m_LookAtPoint.z != m_TargetPoint.z )
	{
		m_LookAtPoint.z = m_TargetPoint.z;
		m_ActionStatus = ACTION_STATUS_ROTATE;
	}
}

void Unit::MoveToDestination()
{
	float time = static_cast<float>( Timer::GetInstance()->GetElapsedTime() );
		
	CollisionManager::GetInstance()->CheckCollision( m_Collision );
	if ( m_Collision->IsCollide() )
	{
		D3DXVECTOR3 rev = m_Collision->GetReverseVector();
		D3DXVECTOR3 view = m_LookAtPoint - m_EyePoint;
		rev.y = view.y = 0;
		D3DXVec3Normalize( &view, &view );

		// Log( "충돌 중!!!! %f %f %f \n", rev.x, rev.y, rev.z );

		if ( D3DXVec3Dot( &rev, &view ) > 0 )
		{
			D3DXVECTOR3 cross;
			D3DXVec3Cross( &cross, &view, &m_UpVector );

			m_EyePoint += cross * static_cast<float>( 10 - m_UnitID )* time / 1000;
			m_LookAtPoint += cross * static_cast<float>( 10 - m_UnitID )* time / 1000;

			// rev = (rev + view) / 100;
			// m_Collision->GetCompetitor()->SetEyePoint( m_Collision->GetCompetitor()->GetEyePoint() - static_cast<float>( 10 - m_UnitID )* rev * time / 1000 );
		}
		else
		{
			m_EyePoint += rev * static_cast<float>( 10 - m_UnitID )* time / 1000;
			m_LookAtPoint += rev * static_cast<float>( 10 - m_UnitID )* time / 1000;
		}
	}

	if ( m_TargetPoint.x - m_EyePoint.x > 0.5f || m_TargetPoint.x - m_EyePoint.x < -0.5f )
	{
		m_EyePoint.x += ( m_TargetPoint.x - m_EyePoint.x ) * time / 1000;
		m_ActionStatus = ACTION_STATUS_GOFOWARD;
	}
	if ( m_TargetPoint.z - m_EyePoint.z > 0.5f || m_TargetPoint.z - m_EyePoint.z < -0.5f )
	{
		m_EyePoint.z += ( m_TargetPoint.z - m_EyePoint.z ) * time / 1000;
		m_ActionStatus = ACTION_STATUS_GOFOWARD;
	}
}

void Unit::ClearAct()
{
	ActDefault act;
	m_Act = act;
}

void Unit::LookForward()
{
	// m_LookAtPoint = m_LookAtPoint * 0.9f + m_Corps->GetLookAtVector() * 0.1f;
}