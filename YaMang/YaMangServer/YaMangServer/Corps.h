#pragma once
#include "SharedDefine.h"
#include "EnumSet.h"
#include "Action.h"

extern const float NOT_A_NUMBER_FLOAT;
class Corps
{
public:
	Corps( int playerID, int corpsID, PositionInfo position );
	virtual ~Corps();

	int						GetCorpsID() { return m_CorpsID; }
	short					GetCorpsSize() { return m_UnitNum; }
	PositionInfo			GetPosition() { return m_Position; }

	void					SetHoldingAction( Action* holdAction ) { m_HoldAction = holdAction; }
	Action*					GetHoldingAction( ) { return m_HoldAction; }

	void					ChangeFormation( FormationType formation ) { m_Formation = formation; }
	void					AddDamage( int damage );

	
private:
	Corps();
	Corps( const Corps& );
	Corps& operator=( const Corps& );

protected:
	int						m_PlayerID = -1;
	int						m_CorpsID = -1;
	bool					m_IsDead = false;
	int						m_HP = 100;
	short					m_UnitNum = -1;

	PositionInfo			m_Position;
	FormationType			m_Formation = FormationType::FORMATION_NONE;

	Action*					m_HoldAction = nullptr;

	int		m_MoveSpeed = 0;
	int		m_MoveSpeedBonus = 0;

	int		m_AttackRange = 0;
	int		m_AttackPower = 0;
	int		m_AttackPowerBonus = 0;

	int		m_Defense = 0;
	int		m_DefenseBonus = 0;


};

