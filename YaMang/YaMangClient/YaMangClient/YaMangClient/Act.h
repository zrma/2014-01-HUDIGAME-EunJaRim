#pragma once
#include "Logger.h"
#include "EnumSet.h"

class Unit;

struct Act
{
public:
	virtual void operator()( Unit& ) = 0;
};

struct ActDefault: public Act
{
	virtual void operator() ( Unit& )
	{
		// Log( "디폴트입니다. \n" );
	}
};

struct ActAttack: public Act
{
	virtual void operator() ( Unit& unit )
	{
		Log( "유닛이 공격! \n" );
	}
};

struct ActRotate: public Act
{
	virtual void operator() ( Unit& unit )
	{
		Log( "유닛이 회전! \n" );
	}
};

struct ActMoveForward: public Act
{
	virtual void operator() ( Unit& unit )
	{
		Log( "유닛이 전진! \n" );
	}
};
