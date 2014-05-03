#pragma once
#include "SharedDefine.h"
#include "EnumSet.h"
#include "Action.h"

class ActionScheduler;
class Corps
{
public:
	Corps( int playerID, int corpsID, PositionInfo position, ActionScheduler* actionScheduler );
	virtual ~Corps();

	int						GetCorpsID( ) const { return m_CorpsID; }
	short					GetCorpsSize( ) const { return m_UnitNum; }
	PositionInfo			GetPositionInfo( ) const { return m_Position; }

	void					SetHoldingAction( Action* holdAction ) { m_HoldAction = holdAction; }
	Action*					GetHoldingAction( ) const { return m_HoldAction; }

	void					ChangeFormation( FormationType formation ) { m_Formation = formation; }
	void					AddDamage( int damage );

	
	int						GetSpeed( ) const { return m_MoveSpeed + m_MoveSpeedBonus; }
	int						GetAttackRange( ) const { return m_AttackRange + m_AttackRangeBonus; }
	int						GetAttackPower( ) const { return m_AttackPower + m_AttackPowerBonus; }
	int						GetDefense( ) const { return m_Defense + m_DefenseBonus; }
	bool					IsDead() const { return m_IsDead; }

	void					DoNextAction( Action* addedAction, ULONGLONG remainTime );

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

	int		m_AttackRange = 1;
	int		m_AttackRangeBonus = 0;

	int		m_AttackPower = 0;
	int		m_AttackPowerBonus = 0;

	int		m_Defense = 0;
	int		m_DefenseBonus = 0;

private:
	ActionScheduler*	m_ActionScheduler = nullptr;
};

