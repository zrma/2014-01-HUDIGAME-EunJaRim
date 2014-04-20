#pragma once
#include "Corps.h"

class Sword:
	public Corps
{
public:
	Sword();
	~Sword();

	void	GenerateCorps( int corpsID, Position position );
};

