#include "stdafx.h"
#include "Action.h"
#include "ActionScheduler.h"


Action::Action()
{
}


Action::~Action()
{
}

void Action::ActIt()
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
	// 큐에 재 등록
	m_ActionScheduler->AddActionToScheduler( this, 10 ); ///# 왜 재등록? 액션 객체는 한번 끝나면 소멸되어 없어져야 하는데?
}
