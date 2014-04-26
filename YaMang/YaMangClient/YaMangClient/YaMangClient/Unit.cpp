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
	
	/*if ( !m_Corps )
	{
		assert( false );
	}
	D3DXMATRIXA16 parentMatrix = m_Corps->GetMatrix();
	
	D3DXMATRIXA16 formMatrix;
	D3DXMatrixIdentity( &formMatrix );

	D3DXVECTOR3 formationVector = ( m_Corps->GetFormation( m_UnitId ) ) * 3;
	D3DXMatrixTranslation( &formMatrix, formationVector.x, formationVector.y, formationVector.z );

	formMatrix = formMatrix * parentMatrix;

	D3DXVECTOR3	targetEyePoint = { 0.0f, m_EyePoint.y, 0.0f };
	D3DXVECTOR3 targetLookAtPoint = { 0.0f, m_LookAtPoint.y, 1.0f };
	D3DXVec3TransformCoord( &targetEyePoint, &targetEyePoint, &formMatrix );
	D3DXVec3TransformCoord( &targetLookAtPoint, &targetLookAtPoint, &formMatrix );
	
	float time = static_cast<float>(Timer::GetInstance()->GetElapsedTime());
	
	if ( targetEyePoint.x - m_EyePoint.x > 0.5f || targetEyePoint.x - m_EyePoint.x < -0.5f )
	{
		m_EyePoint.x += ( targetEyePoint.x - m_EyePoint.x ) * time / 1000;
	}
	if ( targetEyePoint.z - m_EyePoint.z > 0.5f || targetEyePoint.z - m_EyePoint.z < -0.5f )
	{
		m_EyePoint.z += ( targetEyePoint.z - m_EyePoint.z ) * time / 1000;
	}

	if ( targetLookAtPoint.x - m_LookAtPoint.x > 0.5f || targetLookAtPoint.x - m_LookAtPoint.x < -0.5f )
	{
		m_LookAtPoint.x += ( targetLookAtPoint.x - m_LookAtPoint.x ) * time / 1000;
	}
	if ( targetLookAtPoint.z - m_LookAtPoint.z > 0.5f || targetLookAtPoint.z - m_LookAtPoint.z < -0.5f )
	{
		m_LookAtPoint.z += ( targetLookAtPoint.z - m_LookAtPoint.z ) * time / 1000;
	}*/
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

	D3DXVECTOR3 formationVector = ( m_Corps->GetFormation( m_UnitId ) ) * 3;
	D3DXMatrixTranslation( &formMatrix, formationVector.x, formationVector.y, formationVector.z );

	formMatrix = formMatrix * parentMatrix;

	D3DXVec3TransformCoord( &m_EyePoint, &m_EyePoint, &formMatrix );
	D3DXVec3TransformCoord( &m_LookAtPoint, &m_LookAtPoint, &formMatrix );
}

void Unit::ClearAct()
{
	ActDefault act; m_Act = act;
}
