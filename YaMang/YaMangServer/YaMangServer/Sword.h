#pragma once
#include "Corps.h"

class Sword:
	public Corps
{
public:
	Sword( int playerID, int corpsID, UnitType unitType, PositionInfo position, GameRoom* clientManager );
	virtual ~Sword( );

};

