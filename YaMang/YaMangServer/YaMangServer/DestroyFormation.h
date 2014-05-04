#pragma once
#include "Action.h"
class DestroyFormation:
	public Action
{
public:
	DestroyFormation();
	virtual ~DestroyFormation( );

private:
	virtual void OnBegin( );
	virtual void OnTick( );
	virtual void OnEnd( );

};

