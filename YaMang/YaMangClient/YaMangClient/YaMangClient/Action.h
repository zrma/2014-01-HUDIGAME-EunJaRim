#pragma once

class Corps;

struct Action
{
public:
	virtual void operator()( const Corps& ) = 0;
};

struct Attack: public Action
{

};

struct CorpsSpawn: public Action
{

};

struct DefenseFormation: public Action
{

};

struct DestroyFormation: public Action
{

};

struct HoldPosition: public Action
{

};

struct MovePosition: public Action
{

};

struct RushFormation: public Action
{

};

struct TakeArea: public Action
{

};
