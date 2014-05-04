#pragma once
#include "Action.h"
class HoldPosition:
	public Action
{
public:
	HoldPosition();
	virtual ~HoldPosition( );

private:
	virtual void OnBegin( );
	virtual void OnTick( );
	virtual void OnEnd( );

};

