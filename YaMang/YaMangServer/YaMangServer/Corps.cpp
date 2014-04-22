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
	///# 경계값 체크해라카마
	m_HP = m_HP - damage;
}
