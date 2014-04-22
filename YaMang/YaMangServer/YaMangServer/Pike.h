#pragma once
#include "Corps.h"

class Pike:
	public Corps
{
public:
	Pike();
	virtual ~Pike( );

	void	GenerateCorps( int corpsID, Position position );
};

