#pragma once
#include "Action.h"
class MovePosition:
	public Action
{
public:
	MovePosition();
	virtual ~MovePosition( );

private:
	void OnBegin();
	void OnTick();
	void OnEnd();
};

