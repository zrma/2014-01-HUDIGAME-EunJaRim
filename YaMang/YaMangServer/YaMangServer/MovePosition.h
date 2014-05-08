﻿#pragma once
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
	virtual void OnBegin( );
	virtual void OnTick( );
	virtual void OnEnd( );

private:
	PositionInfo	m_Destination;


	
	ULONGLONG		m_StartedTime = 0;
	ULONGLONG		m_MovingTime = 0;
	PositionInfo	m_MovingRoute;
};

