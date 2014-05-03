#pragma once
#include "Corps.h"

class Guard:
	public Corps
{
public:
	Guard( int playerID, int corpsID, PositionInfo position, ActionScheduler* actionScheduler );
	virtual ~Guard();

private:
	Guard( );
	Guard( const Guard& );
	Guard& operator=( const Guard& );

};

