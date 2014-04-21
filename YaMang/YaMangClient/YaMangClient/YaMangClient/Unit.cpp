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

	D3DXMATRIXA16 rotateMatrix;
	D3DXMatrixLookAtRH( &rotateMatrix, &m_EyePoint, &m_LookAtPoint, &m_UpVector );
	rotateMatrix._41 = rotateMatrix._42 = rotateMatrix._43 = 0.0f;
	D3DXMatrixTranspose( &rotateMatrix, &rotateMatrix );

	D3DXMATRIXA16 transMatrix;
	D3DXMatrixTranslation( &transMatrix, m_EyePoint.x, m_EyePoint.y, m_EyePoint.z );

	D3DXMATRIXA16 scaleMatrix;
	D3DXMatrixScaling( &scaleMatrix, m_Scale.x, m_Scale.y, m_Scale.z );
	
	D3DXMATRIXA16 worldMatrix;
	D3DXMatrixIdentity( &worldMatrix );

	worldMatrix = scaleMatrix * rotateMatrix * transMatrix;
	Renderer::GetInstance()->SetWorldMatrix( worldMatrix );

	ResourceMesh* mesh = ResourceManager::GetInstance()->GetMeshByKey( m_MeshKey );

	if ( mesh )
	{
		Renderer::GetInstance()->RenderMesh( mesh->m_MeshObject );
	}
}
