#pragma once
#include "Corps.h"

class Pike:
	public Corps
{
public:
	Pike( int playerID, int corpsID, Position position ):Corps( playerID, corpsID, position ) {};
	virtual ~Pike( );

private:
	Pike( );
	Pike( const Pike& );
	Pike& operator=( const Pike& );

};

