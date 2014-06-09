#pragma once
#include "Action.h"
class KnightAttack:
	public Action
{
public:
	KnightAttack();
	virtual ~KnightAttack( );

	void		SetTargetCorps( Corps* targetCrops ) { m_TargetCrops = targetCrops; }
	Corps*		GetTargetCorps() const { return m_TargetCrops; }

private:
	virtual void OnBegin();
	virtual void OnTick();
	virtual void OnEnd();

private:
	Corps*		m_TargetCrops = nullptr;
	bool		m_CanAttack = true;
	ULONGLONG	m_AttackedTime = 0;
};

