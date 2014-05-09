#pragma once
#include "Corps.h"

class Arrow:
	public Corps
{
public:
	Arrow( int playerID, int corpsID, UnitType unitType, PositionInfo position, ClientManager* clientManager );
	virtual ~Arrow();

};

