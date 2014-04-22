#pragma once
#include "Corps.h"

class Sword:
	public Corps
{
public:
	Sword();
	virtual ~Sword( );

	void	GenerateCorps( int corpsID, Position position );
};

