#pragma once
#include "Action.h"

class ClientSession;
class GenerateCorpOnce:
	public Action
{
public:
	GenerateCorpOnce();
	virtual ~GenerateCorpOnce( );

	void			SetPlayerID( int playerID ) { m_PlayerID = playerID; }
	void			SetClientSession( ClientSession* clientSession ) { m_ClientSession = clientSession; }

	void			SetCorpData( const Corps* corps ) { m_Corps = corps; }
	void			SetCorpHP( float HP ) { m_CorpsHP = HP; }

private:
	virtual void OnBegin();
	virtual void OnTick();
	virtual void OnEnd();

private:
	int				m_PlayerID = -1;
	ClientSession*	m_ClientSession = nullptr;

	float			m_CorpsHP = -1.0f;
	const Corps*	m_Corps = nullptr;
};

