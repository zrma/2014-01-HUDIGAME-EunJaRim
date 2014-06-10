#pragma once
#include "Action.h"
class TakeArea:
	public Action
{
public:
	TakeArea();
	virtual ~TakeArea( );

	void		SetTargetCorps( Corps* targetCorps ) { m_TargetCorps = targetCorps; }
	Corps*		GetTargetCorps() const { return m_TargetCorps; }

private:
	virtual void OnBegin( );
	virtual void OnTick( );
	virtual void OnEnd( );

private:
	Corps*		m_TargetCorps = nullptr;
};

