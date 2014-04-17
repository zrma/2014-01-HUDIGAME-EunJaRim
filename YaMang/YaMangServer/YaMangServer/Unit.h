#pragma once


extern const float NOT_A_NUMBER_FLOAT;
class Unit
{
public:
	Unit();
	virtual ~Unit();


private:
	
	float	m_PosX = NOT_A_NUMBER_FLOAT;
	float	m_PosY = NOT_A_NUMBER_FLOAT;
	int		m_UnitId = -1;


	int		m_AttackRange = -1;
	int		m_AttackPower = -1;
	int		m_AttackPowerBonus = 0;

	int		m_Defense = -1;
	int		m_DefenseBonus = 0;

	int		m_MoveSpeed = -1;
	int		m_MoveSpeedBonus = 0;


};

