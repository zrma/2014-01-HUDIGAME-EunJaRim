#include "stdafx.h"
#include "Frustum.h"


Frustum::Frustum()
{
	ZeroMemory( m_Vertex, sizeof( m_Vertex[0] ) * 8 );
	ZeroMemory( m_Plane, sizeof( m_Plane[0] ) * 6 );
}


Frustum::~Frustum()
{
}

BOOL Frustum::Make( D3DXMATRIXA16* viewProjMatrix )
{
	D3DXMATRIXA16	invMatrix;

	m_Vertex[0].x = -1.0f;	m_Vertex[0].y = -1.0f;	m_Vertex[0].z = 0.0f;
	m_Vertex[1].x = 1.0f;	m_Vertex[1].y = -1.0f;	m_Vertex[1].z = 0.0f;
	m_Vertex[2].x = 1.0f;	m_Vertex[2].y = -1.0f;	m_Vertex[2].z = 1.0f;
	m_Vertex[3].x = -1.0f;	m_Vertex[3].y = -1.0f;	m_Vertex[3].z = 1.0f;
	m_Vertex[4].x = -1.0f;	m_Vertex[4].y = 1.0f;	m_Vertex[4].z = 0.0f;
	m_Vertex[5].x = 1.0f;	m_Vertex[5].y = 1.0f;	m_Vertex[5].z = 0.0f;
	m_Vertex[6].x = 1.0f;	m_Vertex[6].y = 1.0f;	m_Vertex[6].z = 1.0f;
	m_Vertex[7].x = -1.0f;	m_Vertex[7].y = 1.0f;	m_Vertex[7].z = 1.0f;

	D3DXMatrixInverse( &invMatrix, NULL, viewProjMatrix );

	for ( int i = 0; i < 8; ++i )
	{
		D3DXVec3TransformCoord( &m_Vertex[i], &m_Vertex[i], &invMatrix );
	}
	
	m_ViewerPosition = ( m_Vertex[0] + m_Vertex[5] ) / 2.0f;

	//	D3DXPlaneFromPoints(&m_plane[0], m_vtx+4, m_vtx+7, m_vtx+6);				// 상 평면(top)
	//	D3DXPlaneFromPoints(&m_plane[1], m_vtx  , m_vtx+1, m_vtx+2);				// 하 평면(bottom)
	//	D3DXPlaneFromPoints(&m_plane[2], m_vtx  , m_vtx+4, m_vtx+5);				// 근 평면(near)
	D3DXPlaneFromPoints( &m_Plane[3], m_Vertex + 2, m_Vertex + 6, m_Vertex + 7 );	// 원 평면(far)
	D3DXPlaneFromPoints( &m_Plane[4], m_Vertex, m_Vertex + 3, m_Vertex + 7 );		// 좌 평면(left)
	D3DXPlaneFromPoints( &m_Plane[5], m_Vertex + 1, m_Vertex + 5, m_Vertex + 6 );	// 우 평면(right)

	return TRUE;
}

BOOL Frustum::IsIn( D3DXVECTOR3* point )
{
	float distance = 0;
	
	// 현재는 left, right, far plane만 적용한다.
	//	for( int i = 0 ; i < 6 ; ++i )
	{
		distance = D3DXPlaneDotCoord( &m_Plane[3], point );
		if ( distance > PLANE_EPSILON )
		{
			// plane의 normal벡터가 far로 향하고 있으므로 양수이면 프러스텀의 바깥쪽
			return FALSE;
		}
		distance = D3DXPlaneDotCoord( &m_Plane[4], point );
		if ( distance > PLANE_EPSILON )
		{
			// plane의 normal벡터가 left로 향하고 있으므로 양수이면 프러스텀의 왼쪽
			return FALSE;
		}
		distance = D3DXPlaneDotCoord( &m_Plane[5], point );
		if ( distance > PLANE_EPSILON )
		{
			// plane의 normal벡터가 right로 향하고 있으므로 양수이면 프러스텀의 오른쪽
			return FALSE;
		}
	}

	return TRUE;
}

BOOL Frustum::IsInSphere( D3DXVECTOR3* point, float radius )
{
	float distance = 0;

	distance = D3DXPlaneDotCoord( &m_Plane[3], point );
	if ( distance > ( radius + PLANE_EPSILON ) )
	{
		// 평면과 중심점의 거리가 반지름보다 크면 프러스텀에 없음
		return FALSE;
	}
	distance = D3DXPlaneDotCoord( &m_Plane[4], point );
	if ( distance > ( radius + PLANE_EPSILON ) )
	{
		// 평면과 중심점의 거리가 반지름보다 크면 프러스텀에 없음
		return FALSE;
	}
	distance = D3DXPlaneDotCoord( &m_Plane[5], point );
	if ( distance > ( radius + PLANE_EPSILON ) )
	{
		// 평면과 중심점의 거리가 반지름보다 크면 프러스텀에 없음
		return FALSE;
	}

	return TRUE;
}
