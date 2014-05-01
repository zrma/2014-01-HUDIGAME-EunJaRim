#pragma once
#include "Corps.h"

class Guard:
	public Corps
{
public:
	Guard( int playerID, int corpsID, PositionInfo position, ActionScheduler* actionScheduler ):Corps( playerID, corpsID, position, actionScheduler ) {};
	virtual ~Guard();

private:
	Guard( );
	Guard( const Guard& );
	Guard& operator=( const Guard& );

};

