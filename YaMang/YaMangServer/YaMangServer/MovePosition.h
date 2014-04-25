#pragma once
#include "Action.h"
class MovePosition:
	public Action
{
public:
	MovePosition();
	~MovePosition();

	void OnBegin();
	void OnTick();
	void OnEnd();
};

