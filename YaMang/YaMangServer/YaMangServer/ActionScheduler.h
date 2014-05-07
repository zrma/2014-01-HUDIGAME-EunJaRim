#pragma once

class Action;
class ClientManager;

struct ActionComparator
{
	bool operator()( const Action* lhs, const Action* rhs ) const
	{
		return ( lhs->GetTime() > rhs->GetTime() );
	}
};

typedef std::chrono::high_resolution_clock Clock; ///# 안쓰면 지울 것

class ActionScheduler
{
public:
	ActionScheduler( ClientManager* clientManager );
	~ActionScheduler();

	void	AddActionToScheduler( Action* addedAction, ULONGLONG remainTime );
	void	DoScheduledAction();

private:
	ULONGLONG		GetCurrentTick() const;

private:
	ULONGLONG		m_BeginTime;
	ULONGLONG		m_CurrentTime = 0;

	std::priority_queue<Action*, std::vector<Action*>, ActionComparator>	m_ActionQueue;

	ClientManager*	m_ClientManager = nullptr;
};

