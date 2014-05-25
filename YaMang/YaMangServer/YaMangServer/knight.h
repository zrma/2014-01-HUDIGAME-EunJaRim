#pragma once
#include "Corps.h"

class Knight:
	public Corps
{
public:
	Knight( int playerID, int corpsID, UnitType unitType, PositionInfo position, GameRoom* gameRoom );
	virtual ~Knight( );

};

