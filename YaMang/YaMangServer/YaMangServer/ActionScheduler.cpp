#include "stdafx.h"
#include "Action.h"
#include "ActionScheduler.h"
#include "ClientManager.h"
#include "Corps.h"


ActionScheduler::ActionScheduler( ClientManager* clientManager ):
m_ClientManager( clientManager )
{
	m_BeginTime = GetTickCount64( );
}


ActionScheduler::~ActionScheduler()
{
	m_BeginTime = GetTickCount64( );
	m_CurrentTime = GetTickCount64( );
}

ULONGLONG ActionScheduler::GetCurrentTick( )
{
	return GetTickCount64( ) - m_BeginTime;
}

void ActionScheduler::AddActionToScheduler( Action* addedAction, ULONGLONG remainTime )
{
	ULONGLONG workingTime = remainTime + m_CurrentTime;
	addedAction->SetTime( workingTime );
	addedAction->SetScheduler( this );
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

			// 처음 액션을 받는 콥스가 아닐 경우
			if ( nullptr != holdingAction && headAction != holdingAction)
			{
				holdingAction->LowKick();
			}
			corp->SetHoldingAction( headAction );

			// 이 액션의 주기적인 실행은 누가하지??;;;
			headAction->DoAction();
		}

	}
}

