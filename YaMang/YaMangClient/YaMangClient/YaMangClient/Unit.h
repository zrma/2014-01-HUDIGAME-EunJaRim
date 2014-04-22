#pragma once
#include "Interface3D.h"
#include "EnumSet.h"

class Corps;

class Unit: public Interface3D
{
public:
	Unit( Corps* owner, UINT unitId )
		: m_Corps( owner ), m_UnitId( unitId ) {}
	virtual ~Unit();

	virtual void	Update();
	virtual void	Render();
	
	int			GetUnitId() { return m_UnitId; }

protected:
	int			m_UnitId = -1;
	MeshKeyType	m_MeshKey = MESH_KEY_NONE;
	Corps*		m_Corps = nullptr;

private:
	Unit();
	Unit( const Unit& );
	Unit& operator=( const Unit& );
};