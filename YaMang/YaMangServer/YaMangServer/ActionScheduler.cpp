#include "stdafx.h"
#include "Action.h"
#include "ActionScheduler.h"
#include "ClientManager.h"
#include "Corps.h"


ActionScheduler::ActionScheduler( ClientManager* clientManager ):
m_ClientManager( clientManager )
{
	m_BeginTime = Clock::now();
}


ActionScheduler::~ActionScheduler()
{
	m_BeginTime = Clock::now();
	m_CurrentTime = GetCurrentTick();
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

		// 일단 위의 검증이 끝났으므로, 우선순위 큐에서 뽑는다.
		m_ActionQueue.pop();

		// @author 신동찬
		// Action을 뜯어서 상태를 확인
		// 상태가 영 좋지 않은 곳에 맞았다면 제거
		if ( headAction->Gozarani() )
		{
			delete headAction;
			continue;
		}
		// 상태가 좋으면 owner corps id 탐색 후 action 지정

		int ownerCorpsID = headAction->GetOwnerCorpsID();
		Corps* corp = m_ClientManager->GetCorpsByCorpsID( ownerCorpsID );
		
		if ( nullptr != corp )
		{
			Action* holdingAction = corp->GetHoldingAction();

			// 처음 액션을 받는 콥스일경우
			if ( nullptr != holdingAction )
			{
				holdingAction->LowKick();
			}
			corp->SetHoldingAction( headAction );
		}

	}
}

