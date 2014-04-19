#include "stdafx.h"
#include "Unit.h"
#include "SharedDefine.h"
#include "Renderer.h"
#include "ResourceManager.h"

Unit::Unit()
{
}


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

	D3DXMATRIXA16 worldMatrix, invMatrix;
	D3DXMatrixLookAtLH( &worldMatrix, &m_EyePoint, &m_LookAtPoint, &m_UpVector );
	D3DXMatrixInverse( &invMatrix, NULL, &worldMatrix );

	D3DXMATRIXA16 scaleMatrix;
	D3DXMatrixScaling( &scaleMatrix, m_Scale.x, m_Scale.y, m_Scale.z );
	D3DXMatrixMultiply( &worldMatrix, &scaleMatrix, &invMatrix );

	Renderer::GetInstance()->SetWorldMatrix( worldMatrix );

	ResourceMesh* mesh = ResourceManager::GetInstance()->GetMeshByKey( m_MeshKey );

	if ( mesh )
	{
		Renderer::GetInstance()->RenderMesh( mesh->m_MeshObject );
	}
}
