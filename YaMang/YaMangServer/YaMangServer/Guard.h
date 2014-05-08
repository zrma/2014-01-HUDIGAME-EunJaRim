#pragma once
#include "Corps.h"

class Guard:
	public Corps
{
public:
	Guard( int playerID, int corpsID, PositionInfo position, ClientManager* clientManager );
	virtual ~Guard();

};

