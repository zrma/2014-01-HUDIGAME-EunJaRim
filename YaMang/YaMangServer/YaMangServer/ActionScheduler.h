#pragma once

#include "Action.h"

class GameRoom;

struct ActionComparator
{
	bool operator()( const std::shared_ptr<Action> lhs, const std::shared_ptr<Action> rhs ) const
	{

		printf_s( "asd" );
		return ( lhs->GetTime() > rhs->GetTime() );
	}
};

class ActionScheduler
{
public:
	ActionScheduler( GameRoom* clientManager );
	~ActionScheduler();

	void	AddActionToScheduler( std::shared_ptr<Action> addedAction, ULONGLONG remainTime );
	void	DoScheduledAction();

private:
	ULONGLONG		GetCurrentTick() const;

private:
	ULONGLONG		m_BeginTime;
	ULONGLONG		m_CurrentTime = 0;

	std::priority_queue< std::shared_ptr<Action>, std::vector< std::shared_ptr<Action>>, ActionComparator>	m_ActionQueue;

	GameRoom*	m_ClientManager = nullptr;
};

