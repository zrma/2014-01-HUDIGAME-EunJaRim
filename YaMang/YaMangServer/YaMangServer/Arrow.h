#pragma once
#include "Corps.h"

class Arrow:
	public Corps
{
public:
	Arrow();
	virtual ~Arrow( );

	void	GenerateCorps( int corpsID, Position position );
};

