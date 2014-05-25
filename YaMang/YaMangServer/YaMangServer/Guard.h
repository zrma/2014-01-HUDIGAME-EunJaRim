#pragma once
#include "Corps.h"

class Guard:
	public Corps
{
public:
	Guard( int playerID, int corpsID, UnitType unitType, PositionInfo position, GameRoom* gameRoom );
	virtual ~Guard();

};

