#pragma once

class Unit;

class Corps
{
public:
	Corps();
	~Corps();
	
	int GenerateCorps();

private:
	std::vector<Unit*>		m_UnitList;
	int						m_CorpsId;

};

