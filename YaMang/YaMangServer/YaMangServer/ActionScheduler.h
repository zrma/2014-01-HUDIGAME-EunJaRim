#pragma once

class Action;

struct ActionComparator
{
	bool operator()( const Action* lhs, const Action* rhs ) const
	{
		return ( lhs->GetTime() > rhs->GetTime() );
	}
};

typedef std::chrono::high_resolution_clock Clock;

class ActionScheduler
{
public:
	ActionScheduler();
	~ActionScheduler();

	void	AddActionToScheduler( Action* addedAction, int64_t remainTime );
	void	DoScheduledAction();
		
private:
	int64_t		GetCurrentTick();

	Clock::time_point	m_BeginTime;
	int64_t				m_CurrentTime = 0;

	std::priority_queue<Action*, std::vector<Action*>, ActionComparator>	m_ActionQueue;
};

