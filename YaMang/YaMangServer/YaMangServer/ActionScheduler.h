#pragma once

#include "Action.h"

class GameRoom;

struct ActionComparator
{
	bool operator()( const Action* lhs, const Action* rhs ) const
	{
		return ( lhs->GetTime() > rhs->GetTime() );
	}
};

class ActionScheduler
{
public:
	ActionScheduler( GameRoom* gameRoom );
	~ActionScheduler();

	void	AddActionToScheduler( Action* addedAction, ULONGLONG remainTime );
	void	DoScheduledAction();

private:
	ULONGLONG		GetCurrentTick() const;

private:
	ULONGLONG		m_BeginTime;
	ULONGLONG		m_CurrentTime = 0;

	std::priority_queue<Action*, std::vector<Action*>, ActionComparator>	m_ActionQueue;

	GameRoom*	m_GameRoom = nullptr;

	// 임시 코드
	std::set<Action*>	m_UsedActions;
};

