#pragma once
#include "Corps.h"

class Arrow:
	public Corps
{
public:
	Arrow( int playerID, int corpsID, PositionInfo position ):Corps( playerID, corpsID, position ) {};
	virtual ~Arrow( );

private:
	Arrow();
	Arrow( const Arrow& );
	Arrow& operator=( const Arrow& );

};

