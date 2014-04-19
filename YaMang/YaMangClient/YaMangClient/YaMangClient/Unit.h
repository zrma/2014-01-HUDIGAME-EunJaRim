#pragma once
#include "Interface3D.h"
#include "EnumSet.h"

class Unit: public Interface3D
{
public:
	Unit();
	virtual ~Unit();

	virtual void	Update();
	virtual void	Render();

protected:
	int			m_UnitId = -1;
	MeshKeyType	m_MeshKey = MESH_KEY_NONE;
};