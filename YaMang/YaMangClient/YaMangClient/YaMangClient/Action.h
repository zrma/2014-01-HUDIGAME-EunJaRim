/**
*@file		Action.h
*@brief		Corps가 갖는 단일 행동을 정의합니다.
*@details	Corps의 Update() 내에서 호출(무한루프) 됩니다.
*/

#pragma once
#include "Logger.h"
#include "EnumSet.h"
#include "Corps.h"
#include "Act.h"

/**
*@brief		Action 추상 클래스
*@details	모든 Action은 이 함수 객체(struct)를 상속 받아 구현합니다.
*@author	zrma
*@date		2014-04-25
*/
struct Action
{
public:
	virtual void operator()( Corps& ) = 0;
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
