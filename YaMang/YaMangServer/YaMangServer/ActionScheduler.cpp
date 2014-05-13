#include "stdafx.h"
#include "Action.h"
#include "ActionScheduler.h"
#include "GameRoom.h"
#include "Corps.h"
#include "MacroSet.h"


ActionScheduler::ActionScheduler( GameRoom* clientManager ):
m_ClientManager( clientManager )
{
	m_BeginTime = GetTickCount64( );
}


ActionScheduler::~ActionScheduler()
{
	m_BeginTime = GetTickCount64( );
	m_CurrentTime = GetTickCount64( );
}

ULONGLONG ActionScheduler::GetCurrentTick( ) const
{
	return GetTickCount64( ) - m_BeginTime;
}

void ActionScheduler::AddActionToScheduler( Action* addedAction, ULONGLONG remainTime )
{
	ULONGLONG workingTime = remainTime + GetTickCount64();
	addedAction->SetTime( workingTime );
	m_ActionQueue.push( addedAction );
}

void ActionScheduler::DoScheduledAction()
{
	m_CurrentTime = GetTickCount64();

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

		///# 액션을 지우는 곳이 여기밖에 없으면 안지워 지는 경우 생기지 않는가?
		if ( headAction->Gozarani() )
		{
			SafeDelete( headAction ); ///# safedelete가 아니라 당연히 null이 아닌 포인터가 여기 들어와야 한다.  assert처리.
			continue;
		}
		// 상태가 좋으면 owner corps id 탐색 후 action 지정

		Corps* corp = headAction->GetOwnerCorps( );
		
		if ( nullptr != corp )
		{
			Action* holdingAction = corp->GetHoldingAction();

			// 처음 액션을 받는 콥스가 아닐 경우
			if ( nullptr != holdingAction && headAction != holdingAction)
			{
				holdingAction->LowKick();
			}
			corp->SetHoldingAction( headAction );

			headAction->DoAction();
		}
		else
		{
			// 콥스 없이 움직이는 액션들 generateCorps등등...
			headAction->DoAction( );
		}

	}
}

