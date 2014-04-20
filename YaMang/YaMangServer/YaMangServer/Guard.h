#pragma once
#include "Corps.h"

class Guard:
	public Corps
{
public:
	Guard();
	~Guard();

	void	GenerateCorps( int corpsID, Position position );
};

