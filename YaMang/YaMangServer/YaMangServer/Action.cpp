#include "stdafx.h"
#include "Action.h"


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
