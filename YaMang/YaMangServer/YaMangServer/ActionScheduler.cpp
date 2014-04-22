#include "stdafx.h"
#include "Action.h"
#include "ActionScheduler.h"




ActionScheduler::ActionScheduler()
{
}


ActionScheduler::~ActionScheduler()
{
	m_BeginTime = Clock::now();
	m_CurrentTime = /*ActionScheduler::*/GetCurrentTick();
}

int64_t ActionScheduler::GetCurrentTick()
{
	return std::chrono::duration_cast<std::chrono::milliseconds>( Clock::now() - m_BeginTime ).count();
}

void ActionScheduler::AddActionToScheduler( Action* addedAction, int64_t remainTime )
{
	int64_t workingTime = remainTime + m_CurrentTime;
	addedAction->SetTime( workingTime );
	m_ActionQueue.push( addedAction );
}

void ActionScheduler::DoScheduledAction()
{
	m_CurrentTime = GetCurrentTick();

	while ( !m_ActionQueue.empty() )
	{
		Action* headAction = m_ActionQueue.top();

		if ( m_CurrentTime < (headAction->GetTime()) )
		{
			break;
		}

		//Action을 뜯어서 상태를 확인
		//상태가 영 좋지 않은 곳에 맞았다면 제거
		if ( headAction->Gozarani() )
		{
			delete headAction;
			continue;
		}
		//상태가 좋으면 owner corps id 탐색 후 action 지정

		int ownerID = headAction->GetOwnerCorpsID();

	}
}

