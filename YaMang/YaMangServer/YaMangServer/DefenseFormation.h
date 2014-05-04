#pragma once
#include "Action.h"
class DefenseFormation:
	public Action
{
public:
	DefenseFormation();
	virtual ~DefenseFormation( );

private:
	void OnBegin();
	void OnTick();
	void OnEnd();

};

