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
	void					SetPositionInfo( PositionInfo position ) { m_Position = position; };

	void					SetHoldingAction( Action* holdAction ) { m_HoldAction = holdAction; }
	Action*					GetHoldingAction( ) const { return m_HoldAction; }

	void					ChangeFormation( FormationType formation ) { m_Formation = formation; }
	void					AddDamage( float damage );

	
	float					GetSpeed( ) const { return m_MoveSpeed + m_MoveSpeedBonus; }
	float					GetAttackRange( ) const { return m_AttackRange + m_AttackRangeBonus; }
	float					GetAttackPower( ) const { return m_AttackPower + m_AttackPowerBonus; }
	float					GetDefense( ) const { return m_Defense + m_DefenseBonus; }
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
	short					m_UnitNum = -1;

	PositionInfo			m_Position;
	FormationType			m_Formation = FormationType::FORMATION_NONE;

	Action*					m_HoldAction = nullptr;

	float					m_HP = 100.0f;
	float					m_MoveSpeed = 0.0f;
	float					m_MoveSpeedBonus = 0.0f;

	float					m_AttackRange = 0.0f;
	float					m_AttackRangeBonus = 0.0f;

	float					m_AttackPower = 0.0f;
	float					m_AttackPowerBonus = 0.0f;

	float					m_Defense = 0.0f;
	float					m_DefenseBonus = 0.0f;

private:
	ActionScheduler*	m_ActionScheduler = nullptr;
};

