#pragma once
#include "Action.h"
class KnightAttack:
	public Action
{
public:
	KnightAttack();
	virtual ~KnightAttack( );

	void		SetTargetCorps( Corps* targetCorps ) { m_TargetCorps = targetCorps; }
	Corps*		GetTargetCorps() const { return m_TargetCorps; }

private:
	virtual void OnBegin();
	virtual void OnTick();
	virtual void OnEnd();

private:
	Corps*		m_TargetCorps = nullptr;
	bool		m_CanAttack = true;
	ULONGLONG	m_AttackedTime = 0;
};

