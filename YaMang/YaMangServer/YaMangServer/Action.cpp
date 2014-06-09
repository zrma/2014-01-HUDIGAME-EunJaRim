#include "stdafx.h"
#include "Action.h"
#include "ActionScheduler.h"
#include "GameRoom.h"

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
			LowKick();
			break;
	}
}

void Action::OnEnd()
{
	LowKick();
}
