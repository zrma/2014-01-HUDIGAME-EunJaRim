#include "stdafx.h"
#include "Corps.h"
#include "Arrow.h"
#include "Guard.h"
#include "Sword.h"
#include "Pike.h"
#include "knight.h"
#include "EnumSet.h"

Corps::Corps()
{
}


Corps::~Corps()
{
}

void Corps::AddDamage( int damage )
{
	m_HP = m_HP - damage;
}
