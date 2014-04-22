#pragma once
#include "Corps.h"

class Guard:
	public Corps
{
public:
	Guard( int corpsID, Position position ):Corps( corpsID, position ) {};
	virtual ~Guard();

	void	GenerateCorps( int corpsID, Position position );

private:
	Guard();
};

