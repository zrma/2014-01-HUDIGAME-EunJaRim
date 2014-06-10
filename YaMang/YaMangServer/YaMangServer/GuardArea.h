#pragma once
#include "Action.h"
class GuardArea:
	public Action
{
public:
	GuardArea();
	virtual ~GuardArea( );

	void		SetTargetCorps( Corps* targetCorps );
	Corps*		GetTargetCorps() const { return m_TargetCorps; }

private:
	virtual void OnBegin();
	virtual void OnTick();
	virtual void OnEnd();

	void		ReturnMyBase();

private:
	Corps*		m_TargetCorps = nullptr;
	bool		m_GuardModeOn = false;
};

