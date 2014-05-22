#include "stdafx.h"
#include "Action.h"
#include "ActionScheduler.h"
#include "GameRoom.h"
#include "Exception.h"

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

	CRASH_ASSERT( m_ClientManager != nullptr );
	m_ClientManager->AddActionToScheduler( std::shared_ptr<Action>(this), 0 );
}
