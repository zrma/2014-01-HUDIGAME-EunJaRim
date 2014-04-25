#pragma once
#include "Logger.h"
#include "EnumSet.h"

class Corps;

struct Action
{
public:
	virtual void operator()( const Corps& ) = 0;
};

struct ActionDefault: public Action
{
	virtual void operator() ( const Corps& )
	{
		// Log( "디폴트입니다. \n" );
	}
};

struct ActionAttack: public Action
{
	virtual void operator() ( const Corps& )
	{
		Log( "공격! \n" );
	}
};

struct ActionCorpsSpawn: public Action
{

};

struct ActionDefenseFormation: public Action
{

};

struct ActionDestroyFormation: public Action
{

};

struct ActionHoldPosition: public Action
{

};

struct ActionMovePosition: public Action
{

};

struct ActionTakeArea: public Action
{

};
