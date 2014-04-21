#pragma once
#include "Interface3D.h"
#include "EnumSet.h"

class Corps;

class Unit: public Interface3D
{
public:
	Unit();
	virtual ~Unit();

	virtual void	Update();
	virtual void	Render();
	
	void		SetSelfCorps( Corps* corps ) { m_Corps = corps; }

	void		SetUnitId( int unitId ) { m_UnitId = unitId; }
	int			GetUnitId() { return m_UnitId; }

protected:
	int			m_UnitId = -1;
	MeshKeyType	m_MeshKey = MESH_KEY_NONE;
	Corps*		m_Corps = nullptr;
};