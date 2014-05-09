#pragma once
#include "Action.h"
class TakeArea:
	public Action
{
public:
	TakeArea();
	virtual ~TakeArea( );

	void		SetTargetCorps( Corps* targetCrops ) { m_TargerCrops = targetCrops; }
	Corps*		GetTargetCorps() const { return m_TargerCrops; }

private:
	virtual void OnBegin( );
	virtual void OnTick( );
	virtual void OnEnd( );

private:
	Corps*		m_TargerCrops = nullptr;
};

