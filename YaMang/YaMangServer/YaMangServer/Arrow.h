#pragma once
#include "Corps.h"

class Arrow:
	public Corps
{
public:
	Arrow();
	~Arrow();

	void	GenerateCorps( int corpsID, Position position );
};

