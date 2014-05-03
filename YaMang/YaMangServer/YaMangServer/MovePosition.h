#pragma once
#include "Action.h"
#include "SharedDefine.h"

class MovePosition:
	public Action
{
public:
	MovePosition();
	virtual ~MovePosition( );

	void	SetDestination( PositionInfo destination ) { m_Destination = destination; };

private:
	void OnBegin();
	void OnTick();
	void OnEnd();

private:
	PositionInfo m_Destination;
};

