#pragma once
#include <d3dx9math.h>

class Interface3D;

class Collision
{
public:
	Collision( Interface3D* owner, float radius ): m_Owner(owner), m_Radius(radius) {}
	~Collision();

	float	GetRadius() const { return m_Radius; }
	void	SetRadius( float radius ) { m_Radius = radius; }
	
	D3DXVECTOR3		GetPosition() const;

	bool	IsCollideWith( Collision& collision ) const;
	
private:
	Collision();

	float	m_Radius;
	Interface3D*	m_Owner;
};

