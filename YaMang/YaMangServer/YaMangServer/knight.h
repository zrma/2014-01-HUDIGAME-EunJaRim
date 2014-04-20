#pragma once
#include "Corps.h"

class Knight:
	public Corps
{
public:
	Knight();
	~Knight();

	void	GenerateCorps( int corpsID, Position position );
};

