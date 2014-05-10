#include "stdafx.h"
#include "Collision.h"
#include "Interface3D.h"

Collision::~Collision()
{
}

bool Collision::IsCollideWith( Collision& collision )
{
	if ( nullptr == m_Owner )
	{
		return false;
	}
	
	D3DXVECTOR3 distance;
	D3DXVECTOR3 thisPoint = GetPosition();
	D3DXVECTOR3 targetPoint = collision.GetPosition();
	D3DXVec3Subtract( &distance, &thisPoint, &targetPoint );

	float radiusSq = m_Radius + collision.GetRadius();
	radiusSq *= radiusSq;

	if ( D3DXVec3LengthSq( &distance ) < radiusSq )
	{
		m_ReverseVector = GetPosition() - collision.GetPosition();
		D3DXVec3Normalize( &m_ReverseVector, &m_ReverseVector );

		m_Competitor = collision.GetOwner();
		m_IsCollide = true;

		return m_IsCollide;
	}

	m_IsCollide = false;
	return m_IsCollide;
}

D3DXVECTOR3 Collision::GetPosition() const
{
	if ( nullptr == m_Owner )
	{
		assert( false );
		return{ 0.0f, 0.0f, 0.0f };
	}

	return m_Owner->GetEyePoint();
}
