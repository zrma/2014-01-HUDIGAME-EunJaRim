#pragma once
#include "Corps.h"

class King:
	public Corps
{
public:
	King( int playerID, int corpsID, UnitType unitType, PositionInfo position, GameRoom* gameRoom );
	virtual ~King( );
};

