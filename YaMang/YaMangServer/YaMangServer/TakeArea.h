#pragma once
#include "Action.h"
class TakeArea:
	public Action
{
public:
	TakeArea();
	virtual ~TakeArea( );

private:
	virtual void OnBegin( );
	virtual void OnTick( );
	virtual void OnEnd( );

};

