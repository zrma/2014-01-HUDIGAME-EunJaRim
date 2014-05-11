#pragma once
#include "Corps.h"

class Pike:
	public Corps
{
public:
	Pike( int playerID, int corpsID, UnitType unitType, PositionInfo position, GameRoom* clientManager );
	virtual ~Pike( );

};

