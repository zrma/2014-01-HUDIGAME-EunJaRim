#pragma once
#include "Corps.h"

class Knight:
	public Corps
{
public:
	Knight( int playerID, int corpsID, PositionInfo position, ActionScheduler* actionScheduler ):Corps( playerID, corpsID, position, actionScheduler ) {};
	virtual ~Knight( );

private:
	Knight( );
	Knight( const Knight& );
	Knight& operator=( const Knight& );

};

