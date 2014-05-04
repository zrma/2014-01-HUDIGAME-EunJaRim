#pragma once
#include "Corps.h"

class Pike:
	public Corps
{
public:
	Pike( int playerID, int corpsID, PositionInfo position, ActionScheduler* actionScheduler );
	virtual ~Pike( );

};

