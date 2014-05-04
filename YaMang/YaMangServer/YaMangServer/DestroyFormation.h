#pragma once
#include "Action.h"
class DestroyFormation:
	public Action
{
public:
	DestroyFormation();
	virtual ~DestroyFormation( );

private:
	void OnBegin();
	void OnTick();
	void OnEnd();

};

