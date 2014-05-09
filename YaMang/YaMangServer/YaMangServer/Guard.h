#pragma once
#include "Corps.h"

class Guard:
	public Corps
{
public:
	Guard( int playerID, int corpsID, UnitType unitType, PositionInfo position, ClientManager* clientManager );
	virtual ~Guard();

};

