#include "stdafx.h"
#include "Arrow.h"
#include "Timer.h"


Arrow::Arrow()
{
	m_MeshKey = MESH_KEY_UNIT_ARROW;
	// 토로

	m_EyePoint = { -8.0f, -1.2f, 20.0f };
	m_LookAtPoint = { -7.0f, -1.2f, 19.0f };
	m_UpVector = { 0.0f, 1.0f, 0.0f };
	m_Scale = { 1.0f, 1.0f, 1.0f };
}

Arrow::~Arrow()
{
}

void Arrow::Update()
{
	/*
	D3DXVECTOR3 view = m_LookAtPoint - m_EyePoint;
	D3DXMATRIXA16 matrix;

	UINT time = Timer::GetInstance()->GetElapsedTime();
	float angle = D3DX_PI * static_cast<float>( time ) / 1000;

	D3DXMatrixRotationY( &matrix, angle );
	D3DXVec3TransformCoord( &view, &view, &matrix );

	m_LookAtPoint = m_EyePoint + view;
	*/
	// printf_s( "\n%f %f %f - %f %f %f ", m_EyePoint.x, m_EyePoint.y, m_EyePoint.z, m_LookAtPoint.x, m_LookAtPoint.y, m_LookAtPoint.z );
}
