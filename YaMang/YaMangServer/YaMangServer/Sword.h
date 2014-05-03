#pragma once
#include "Corps.h"

class Sword:
	public Corps
{
public:
	Sword( int playerID, int corpsID, PositionInfo position, ActionScheduler* actionScheduler );
	virtual ~Sword( );

private:
	Sword( );
	Sword( const Sword& );
	Sword& operator=( const Sword& );

};

