#pragma once
#include "Corps.h"

class Arrow:
	public Corps
{
public:
	Arrow();
	~Arrow(); ///# corps 자식들의 소멸자는 모두 virtual

	void	GenerateCorps( int corpsID, Position position );
};

