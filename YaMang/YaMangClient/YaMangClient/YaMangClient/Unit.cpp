#include "stdafx.h"
#include "Corps.h"
#include "Unit.h"
#include "SharedDefine.h"
#include "Renderer.h"
#include "ResourceManager.h"

Unit::~Unit()
{
}

void Unit::Update()
{
}

void Unit::Render()
{
	if ( !m_Visible || !m_MeshKey )
		return;

	if ( !m_Corps )
	{
		assert( false );
	}
	D3DXMATRIXA16 parentMatrix = m_Corps->GetMatrix();

	D3DXMATRIXA16 formMatrix;
	D3DXMatrixIdentity( &formMatrix );

	D3DXVECTOR3 formationVector = ( m_Corps->GetFormation( m_UnitId ) ) * 3;
	D3DXMatrixTranslation( &formMatrix, formationVector.x, formationVector.y, formationVector.z );
	formMatrix = formMatrix * parentMatrix;

	D3DXMATRIXA16 thisMatrix = GetMatrix( false ) * formMatrix;
	Renderer::GetInstance()->SetWorldMatrix( thisMatrix );

	ResourceMesh* mesh = ResourceManager::GetInstance()->GetMeshByKey( m_MeshKey );

	if ( mesh )
	{
		Renderer::GetInstance()->RenderMesh( mesh->m_MeshObject );
	}
}
