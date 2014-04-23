#pragma once
#include "Corps.h"

class Guard:
	public Corps
{
public:
	Guard( int playerID, int corpsID, Position position ):Corps( playerID, corpsID, position ) {};
	virtual ~Guard();

private:
	Guard( );
	Guard( const Guard& );
	Guard& operator=( const Guard& );

};

