#pragma once
#include "Logger.h"
#include "EnumSet.h"
#include "Corps.h"
#include "Act.h"

struct Action
{
public:
	virtual void operator()( Corps& ) {}
};

struct ActionDefault: public Action
{
	virtual void operator() ( Corps& corps )
	{
		ActDefault act;
		corps.SetAct( act );
	}
};

struct ActionHoldPosition: public Action
{
	virtual void operator() ( Corps& corps )
	{
		ActDefault act;
		corps.SetAct( act );
	}
};

struct ActionAttack: public Action
{
	virtual void operator() ( Corps& corps )
	{
		ActAttackSeries act;
		corps.SetAct( act );
	}
};

struct ActionTakeArea: public Action
{
	virtual void operator() ( Corps& corps )
	{
		ActAttackSeries act;
		corps.SetAct( act );
	}
};

struct ActionTransFormation: public Action
{
	virtual void operator() ( Corps& corps )
	{
		ActMoveSeries act;
		corps.SetAct( act );
	}
};

struct ActionCorpsSpawn: public Action
{
	virtual void operator() ( Corps& corps )
	{
		ActMoveSeries act;
		corps.SetAct( act );
	}
};

struct ActionMovePosition: public Action
{
	virtual void operator() ( Corps& corps )
	{
		corps.GoFoward();

		ActMoveSeries act;
		corps.SetAct( act );
	}
};
