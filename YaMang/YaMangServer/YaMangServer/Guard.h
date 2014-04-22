#pragma once
#include "Corps.h"

class Guard:
	public Corps
{
public:
	Guard();
	virtual ~Guard();

	void	GenerateCorps( int corpsID, Position position );
};

