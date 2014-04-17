#pragma once

extern const float NOT_A_NUMBER_FLOAT;

class Unit
{
public:
	Unit();
	virtual ~Unit();

private:
	float	m_PosX = NOT_A_NUMBER_FLOAT;
	float	m_PosY = -NOT_A_NUMBER_FLOAT;
	int		m_UnitId = -1;

	int		m_MoveSpeed = 0;
};