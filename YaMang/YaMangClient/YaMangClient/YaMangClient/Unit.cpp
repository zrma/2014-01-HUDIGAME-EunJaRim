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
