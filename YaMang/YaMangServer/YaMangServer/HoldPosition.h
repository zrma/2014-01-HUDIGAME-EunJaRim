#pragma once
#include "Action.h"
class HoldPosition:
	public Action
{
public:
	HoldPosition();
	virtual ~HoldPosition( );

private:
	void OnBegin();
	void OnTick();
	void OnEnd();

};

