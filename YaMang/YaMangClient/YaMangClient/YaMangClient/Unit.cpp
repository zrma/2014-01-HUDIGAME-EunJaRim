#include "stdafx.h"
#include "Corps.h"
#include "Unit.h"
#include "SharedDefine.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "Timer.h"
#include "Act.h"

Unit::~Unit()
{
}

void Unit::Update()
{
	DoAct();
}

void Unit::Render()
{
	if ( !m_Visible || !m_MeshKey )
		return;

	D3DXMATRIXA16 thisMatrix = GetMatrix( false );
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

	D3DXVECTOR3	targetPoint = { 0.0f, 0.1f, 0.0f };
	D3DXVec3TransformCoord( &m_TargetPoint, &targetPoint, &formMatrix );

// 	if ( m_UnitID == 3 )
// 	{
// 		Log( " %d번 유닛 목표 좌표 : %f %f ", m_UnitID, m_TargetPoint.x, m_TargetPoint.z );
// 		Log( " 룩앳 : %f %f \n", m_LookAtPoint.x, m_LookAtPoint.z );
// 	}
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
