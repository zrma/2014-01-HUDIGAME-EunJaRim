#include "stdafx.h"
#include "Action.h"
#include "ActionScheduler.h"
#include "GameRoom.h"
#include "Corps.h"
#include "MacroSet.h"
#include "Exception.h"


ActionScheduler::ActionScheduler( GameRoom* gameRoom ):
m_GameRoom( gameRoom )
{
	m_BeginTime = GetTickCount64( );
}


ActionScheduler::~ActionScheduler()
{
	m_BeginTime = GetTickCount64( );
	m_CurrentTime = GetTickCount64( );
	
	for ( auto& it : m_UsedActions )
	{
		Action* toBeDelete = it;
		delete toBeDelete;
	}
}

ULONGLONG ActionScheduler::GetCurrentTick( ) const
{
	return GetTickCount64( ) - m_BeginTime;
}

void ActionScheduler::AddActionToScheduler( Action* addedAction, ULONGLONG remainTime )
{
	m_UsedActions.insert( addedAction );

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
		
		CRASH_ASSERT( headAction != nullptr );

		if ( m_CurrentTime < (headAction->GetTime()) )
		{
			break;
		}

		// 일단 위의 검증이 끝났으므로, 우선순위 큐에서 뽑는다.
		m_ActionQueue.pop();

		// @author 신동찬
		// Action을 뜯어서 상태를 확인
		// 상태가 영 좋지 않은 곳에 맞았다면 제거
		if ( headAction->Gozarani( )  )
		{
			headAction->ClearOwnerCorps();
			// delete headAction; // 방이 종료될때 다 정리 m_UsedActions에서
			continue;
		}

		if ( ACTION_END == headAction->GetActionStatus( ) )
		{
			headAction->DoAction();
			// delete headAction;
			continue;
		}
		
		// 상태가 좋으면 owner corps id 탐색 후 action 지정

		Corps* corp = headAction->GetOwnerCorps( );
		
		if ( corp )
		{
			Action* holdingAction = corp->GetHoldingAction();

			// 처음 액션을 받는 콥스가 아닐 경우
			if ( holdingAction && headAction != holdingAction)
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

