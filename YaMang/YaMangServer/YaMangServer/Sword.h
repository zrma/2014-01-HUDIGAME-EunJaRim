#pragma once
#include "Corps.h"

class Sword:
	public Corps
{
public:
	Sword( int playerID, int corpsID, PositionInfo position, ClientManager* clientManager );
	virtual ~Sword( );

};

