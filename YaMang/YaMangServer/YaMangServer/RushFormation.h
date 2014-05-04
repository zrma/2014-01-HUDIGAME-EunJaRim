#pragma once
#include "Action.h"
class RushFormation:
	public Action
{
public:
	RushFormation();
	virtual ~RushFormation( );

private:
	void OnBegin();
	void OnTick();
	void OnEnd();

};

