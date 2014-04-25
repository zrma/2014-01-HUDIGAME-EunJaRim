#pragma once
#include "Logger.h"
#include "EnumSet.h"
#include "Corps.h"

struct Action
{
public:
	virtual void operator()( Corps& ) {}
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
	virtual void operator() ( Corps& corps )
	{
		Log( "%d번 부대가 공격! \n", corps.GetCorpsID() );
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
	virtual void operator() ( Corps& corps )
	{
		Log( "%d번 부대가 공격! \n", corps.GetCorpsID() );
	}
};

struct ActionTakeArea: public Action
{

};
