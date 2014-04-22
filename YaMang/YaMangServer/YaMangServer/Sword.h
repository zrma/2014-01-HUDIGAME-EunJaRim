#pragma once
#include "Corps.h"

class Sword:
	public Corps
{
public:
	Sword( int corpsID, Position position ):Corps( corpsID, position ) {};
	virtual ~Sword( );

	void	GenerateCorps( int corpsID, Position position );

private:
	Sword( );
	Sword( const Sword& );
	Sword& operator=( const Sword& );

};

