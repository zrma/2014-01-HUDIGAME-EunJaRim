#pragma once
#include "Action.h"
class ChangeFormation:
	public Action
{
public:
	ChangeFormation();
	virtual ~ChangeFormation( );

private:
	virtual void OnBegin();
	virtual void OnTick();
	virtual void OnEnd();

};

