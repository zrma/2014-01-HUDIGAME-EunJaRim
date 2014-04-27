#include "stdafx.h"
#include "Corps.h"
#include "Arrow.h"
#include "Guard.h"
#include "Sword.h"
#include "Pike.h"
#include "knight.h"
#include "EnumSet.h"

Corps::Corps( int playerID, int corpsID, PositionInfo position )
: m_PlayerID( playerID ), m_CorpsID( corpsID ), m_Position(position)
{
}


Corps::~Corps()
{
}

void Corps::AddDamage( int damage )
{
	m_HP = m_HP - damage;
}
