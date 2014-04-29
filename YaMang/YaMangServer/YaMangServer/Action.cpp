#include "stdafx.h"
#include "Action.h"
#include "ActionScheduler.h"


Action::Action()
{
}


Action::~Action()
{
}

void Action::DoAction()
{
	switch ( m_ActionStatus )
	{
		case ACTION_BEGIN:
			OnBegin();
			break;
		case ACTION_TICK:
			OnTick();
			break;
		case ACTION_END:
			OnEnd();
			break;
	}
}

void Action::OnEnd()
{
	LowKick();
	// 큐에 재 등록 delete를 위해 다시 보낼려고 했는데 필요없을까? 생각해보니 다른 액션이 들어오면 corps에 가지고 있는 액션으로 들어와서 애를 지워줄지도 일단 지금 좀 멘붕이니 나중에 생각하자 
	m_ActionScheduler->AddActionToScheduler( this, 10 );
}
