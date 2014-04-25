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

//////////////////////////////////////////////////////////////////////////
// 공격 형태의 FSM을 구현합니다.
//
// 공격 명령, 이동공격(적을 직접 어택해서 거리 좁혀가면서 이동이나 추적 할 때) 등등
//////////////////////////////////////////////////////////////////////////
struct ActAttackSeries: public Act
{
	virtual void operator() ( Unit& unit )
	{
		Log( "유닛이 공격! \n" );
	}
};

//////////////////////////////////////////////////////////////////////////
// 이동 형태의 FSM을 구현합니다.
//
// 진형 변경, 전진, 회전 등등 모두 이곳에서 처리
//////////////////////////////////////////////////////////////////////////
struct ActMoveSeries: public Act
{
	virtual void operator() ( Unit& unit )
	{
		Log( "유닛이 이동! \n" );
	}
};