#pragma once
#include "EnumSet.h"
#include "Interface3D.h"

class Unit;

class Corps: public Interface3D
{
public:
	Corps();
	~Corps();

	void	Update();
	void	Render();
	void	Create(UINT num, UnitType unitType );
	void	SetVisible( bool visible );

private:
	std::vector<Unit*>		m_UnitList;
	int						m_CorpsId = -1;
	int						m_OwnerPlayerID = -1;
};

