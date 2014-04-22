#pragma once
#include "Corps.h"

class Pike:
	public Corps
{
public:
	Pike( int corpsID, Position position ):Corps( corpsID, position ) {};
	virtual ~Pike( );

	void	GenerateCorps( int corpsID, Position position );

private:
	Pike();
};

