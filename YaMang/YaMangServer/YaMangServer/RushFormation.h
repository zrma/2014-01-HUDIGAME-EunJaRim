#pragma once
#include "Action.h"
class RushFormation:
	public Action
{
public:
	RushFormation();
	virtual ~RushFormation( );

private:
	virtual void OnBegin( );
	virtual void OnTick( );
	virtual void OnEnd( );

};

