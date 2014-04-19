#pragma once
#include "Interface3D.h"
#include "Unit.h"

class Corps: public Interface3D
{
public:
	Corps();
	~Corps();

private:
	std::vector<Unit*>		m_UnitList;
	int						m_CorpsId;
};

