#pragma once
#include "Corps.h"

class Knight:
	public Corps
{
public:
	Knight( int corpsID, Position position ):Corps( corpsID, position ) {};
	virtual ~Knight( );

	void	GenerateCorps( int corpsID, Position position );

private:
	Knight();
};

