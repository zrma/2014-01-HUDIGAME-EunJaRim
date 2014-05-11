#pragma once
#include "Corps.h"

class Arrow:
	public Corps
{
public:
	Arrow( int playerID, int corpsID, UnitType unitType, PositionInfo position, GameRoom* clientManager );
	virtual ~Arrow();

};

