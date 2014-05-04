#pragma once
#include "Action.h"
class TakeArea:
	public Action
{
public:
	TakeArea();
	virtual ~TakeArea( );

private:
	void OnBegin();
	void OnTick();
	void OnEnd();

};

