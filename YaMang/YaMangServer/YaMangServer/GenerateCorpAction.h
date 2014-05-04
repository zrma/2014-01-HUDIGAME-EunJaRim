#pragma once
#include "Action.h"

class ClientSession;
class GenerateCorpAction:
	public Action
{
public:
	GenerateCorpAction( );
	virtual ~GenerateCorpAction( );

	void			SetPlayerID( int playerID ) { m_PlayerID = playerID; }
	void			SetClientSession( ClientSession* clientSession ) { m_ClientSession = clientSession; }

private:
	void OnBegin();
	void OnTick();
	void OnEnd();

private:
	int				m_PlayerID = -1;
	ClientSession*	m_ClientSession = nullptr;
};

