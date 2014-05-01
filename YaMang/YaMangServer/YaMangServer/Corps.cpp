#include "stdafx.h"
#include "Corps.h"
#include "Arrow.h"
#include "Guard.h"
#include "Sword.h"
#include "Pike.h"
#include "knight.h"
#include "EnumSet.h"
#include "ActionScheduler.h"

Corps::Corps( int playerID, int corpsID, PositionInfo position, ActionScheduler* actionScheduler )
: m_PlayerID( playerID ), m_CorpsID( corpsID ), m_Position( position ), m_ActionScheduler(actionScheduler)
{
}


Corps::~Corps()
{
}

void Corps::AddDamage( int damage )
{
	if ( !m_IsDead )
	{
		int defense = m_Defense + m_DefenseBonus - damage;
		if ( defense < 0 )
		{
			m_HP = m_HP + defense;
			if ( m_HP < 1 )
			{
				m_IsDead = true;
			}
		}
		
	}
}

void Corps::DoNextAction( Action* addedAction, ULONGLONG remainTime )
{
	m_ActionScheduler->AddActionToScheduler( addedAction, remainTime );
}
