#pragma once
#include "Interface3D.h"

class Unit: public Interface3D
{
public:
	Unit();
	virtual ~Unit();

private:
	int		m_UnitId = -1;
};