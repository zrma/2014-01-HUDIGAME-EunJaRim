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

	void					ChangeFormation( FormationType formation ) { m_Formation = formation; }
	void					AddDamage( int damage );

protected:
	int						m_CorpsId = -1;
	int						m_HP = 100;
	short					m_UnitNum = -1;

	Position				m_Position;
	FormationType			m_Formation = FORMATION_NONE;

	int		m_MoveSpeed = 0;
	int		m_MoveSpeedBonus = 0;

	int		m_AttackRange = 0;
	int		m_AttackPower = 0;
	int		m_AttackPowerBonus = 0;

	int		m_Defense = 0;
	int		m_DefenseBonus = 0;


};

