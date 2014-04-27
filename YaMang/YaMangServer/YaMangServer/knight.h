#pragma once
#include "Corps.h"

class Knight:
	public Corps
{
public:
	Knight( int playerID, int corpsID, PositionInfo position ):Corps( playerID, corpsID, position ) {};
	virtual ~Knight( );

private:
	Knight( );
	Knight( const Knight& );
	Knight& operator=( const Knight& );

};

