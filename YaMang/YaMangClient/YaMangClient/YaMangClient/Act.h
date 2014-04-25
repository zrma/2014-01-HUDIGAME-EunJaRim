#pragma once
#include "Logger.h"
#include "EnumSet.h"

class Unit;

struct Act
{
public:
	virtual void operator()( const Unit& ) = 0;
};

struct ActDefault: public Act
{
	virtual void operator() ( const Unit& )
	{
		// Log( "디폴트입니다. \n" );
	}
};

struct ActAttack: public Act
{
	virtual void operator() ( const Unit& unit )
	{
		Log( "유닛이 공격! \n" );
	}
};

struct CorpsSpawn: public Act
{

};

struct DefenseFormation: public Act
{

};

struct DestroyFormation: public Act
{

};

struct HoldPosition: public Act
{

};

struct MovePosition: public Act
{

};

struct RushFormation: public Act
{

};

struct TakeArea: public Act
{

};
