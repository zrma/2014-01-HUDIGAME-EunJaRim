#pragma once
#include "Corps.h"

class Pike:
	public Corps
{
public:
	Pike( int playerID, int corpsID, PositionInfo position, ActionScheduler* actionScheduler ):Corps( playerID, corpsID, position, actionScheduler ) {};
	virtual ~Pike( );

private:
	Pike( );
	Pike( const Pike& );
	Pike& operator=( const Pike& );

};

