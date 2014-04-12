#include "stdafx.h"
#include "GameOverHandler.h"
#include "NetworkManager.h"


GameOverHandler::GameOverHandler()
{
}


GameOverHandler::~GameOverHandler()
{
}

void GameOverHandler::HandleEvent( PacketHeader& pktBase )
{
	GameOverResult recvData = static_cast<GameOverResult&>( pktBase );

	if ( NetworkManager::GetInstance()->m_RecvBuffer.Read( (char*)&recvData, recvData.m_Size ) )
	{
		/// 여기 걸리면 서버로부터 잘못된 아이디가 넘어온것
		assert( recvData.m_PlayerId != -1 );

		if ( NetworkManager::GetInstance( )->m_MyPlayerId != recvData.m_PlayerId )
		{
			return;
		}

		if ( recvData.m_IsWon )
		{
			Log( "I'm WIN!! \n" );
		}
		else
		{
			Log( "I'm Lose... \n" );
		}

	}
	else
	{
		assert( false );
	}
}
