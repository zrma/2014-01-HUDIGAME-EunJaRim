#pragma once
#include "SharedDefine.h"
#include "EnumSet.h"

extern const float NOT_A_NUMBER_FLOAT;
class Corps
{
public:
	Corps( );
	virtual ~Corps();
	
	virtual void			GenerateCorps( int corpsID, Position position ) = 0;

	int						GetCorpsID() { return m_CorpsId; }
	short					GetCorpsSize() { return m_UnitNum; }
	Position				GetPosition() { return m_Position; }

protected:
	int						m_CorpsId;
	int						m_HP;
	short					m_UnitNum;

	Position				m_Position;
	

	float	m_PosX = NOT_A_NUMBER_FLOAT;
	float	m_PosY = NOT_A_NUMBER_FLOAT;

	int		m_AttackRange = 0;
	int		m_AttackPower = 0;
	int		m_AttackPowerBonus = 0;

	int		m_Defense = 0;
	int		m_DefenseBonus = 0;

	int		m_MoveSpeed = 0;
	int		m_MoveSpeedBonus = 0;
};

