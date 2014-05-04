#pragma once
#include "Action.h"
class DefenseFormation:
	public Action
{
public:
	DefenseFormation();
	virtual ~DefenseFormation( );

private:
	virtual void OnBegin( );
	virtual void OnTick( );
	virtual void OnEnd( );

};

