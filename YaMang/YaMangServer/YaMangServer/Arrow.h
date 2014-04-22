#pragma once
#include "Corps.h"

class Arrow:
	public Corps
{
public:
	Arrow( int corpsID, Position position ):Corps( corpsID, position ) {};
	virtual ~Arrow( );

	void	GenerateCorps( int corpsID, Position position );

private:
	Arrow();
	Arrow( const Arrow& );
	Arrow& operator=( const Arrow& );

};

