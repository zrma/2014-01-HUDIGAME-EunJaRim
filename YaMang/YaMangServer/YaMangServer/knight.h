#pragma once
#include "Corps.h"

class Knight:
	public Corps
{
public:
	Knight( int playerID, int corpsID, PositionInfo position, ActionScheduler* actionScheduler );
	virtual ~Knight( );

};

