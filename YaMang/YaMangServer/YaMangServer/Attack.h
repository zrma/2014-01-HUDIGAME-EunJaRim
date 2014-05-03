#pragma once
#include "Action.h"
class Attack:
	public Action
{
public:
	Attack();
	virtual ~Attack();

	void		SetTargetCorps( Corps* targetCrops ) { m_TargerCrops = targetCrops; }
	Corps*		GetTargetCorps( ) const { return m_TargerCrops; }

private:
	void OnBegin();
	void OnTick();
	void OnEnd();

private:
	Corps*		m_TargerCrops = nullptr;
};

