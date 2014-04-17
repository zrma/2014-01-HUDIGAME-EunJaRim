#include "stdafx.h"
#include "PacketType.h"
#include "NetworkManager.h"
#include "GameManager.h"

HandlerFunc HandlerTable[PKT_MAX];

static void DefaultHandler( PacketHeader& pktBase )
{
	assert( false );
	GameManager::GetInstance()->Stop();
}

struct InitializeHandlers
{
	InitializeHandlers()
	{
		for ( int i = 0; i < PKT_MAX; ++i )
		{
			HandlerTable[i] = DefaultHandler;
		}

	}
} _init_handlers_;

struct RegisterHandler
{
	RegisterHandler( int pktType, HandlerFunc handler )
	{
		HandlerTable[pktType] = handler;
	}
};

#define REGISTER_HANDLER(PKT_TYPE)	\
	static void Handler_##PKT_TYPE( PacketHeader& pktBase ); \
	static RegisterHandler _register_##PKT_TYPE( PKT_TYPE, Handler_##PKT_TYPE ); \
	static void Handler_##PKT_TYPE( PacketHeader& pktBase )

REGISTER_HANDLER( PKT_SC_LOGIN )
{
	LoginResult recvData = static_cast<LoginResult&>( pktBase );
	NetworkManager::GetInstance()->HandleLoginResult( recvData );	
}

void NetworkManager::HandleLoginResult( LoginResult& inPacket )
{
	if ( m_RecvBuffer.Read( (char*)&inPacket, inPacket.m_Size ) )
	{
		// 패킷처리
		if ( inPacket.m_PlayerId == -1 )
		{
			/// 여기 걸리면 로그인 실패다.
			GameManager::GetInstance()->Stop();
		}

		printf_s( "My playerID[%d] \n", inPacket.m_PlayerId );
		NetworkManager::GetInstance()->m_MyPlayerId = inPacket.m_PlayerId;
	}
	else
	{
		assert( false );
	}
}

REGISTER_HANDLER( PKT_SC_GAMEOVER )
{
	GameOverResult recvData = static_cast<GameOverResult&>( pktBase );
	NetworkManager::GetInstance()->HandleGameOverResult( recvData );
}

void NetworkManager::HandleGameOverResult( GameOverResult& inPacket )
{
	if ( m_RecvBuffer.Read( (char*)&inPacket, inPacket.m_Size ) )
	{
		/// 여기 걸리면 서버로부터 잘못된 아이디가 넘어온것
		assert( inPacket.m_PlayerId != -1 );

		if ( m_MyPlayerId != inPacket.m_PlayerId )
		{
			return;
		}

		if ( inPacket.m_IsWon )
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

REGISTER_HANDLER( PKT_SC_CHAT )
{
	ChatBroadcastResult recvData = static_cast<ChatBroadcastResult&>( pktBase );
	NetworkManager::GetInstance()->HandleChatResult( recvData );
}

void NetworkManager::HandleChatResult( ChatBroadcastResult& inPacket )
{
	if ( m_RecvBuffer.Read( (char*)&inPacket, inPacket.m_Size ) )
	{
		/// 여기 걸리면 로그인 안된놈이 보낸거다
		assert( inPacket.m_PlayerId != -1 );

		char buff[MAX_CHAT_LEN] = { 0, };
		sprintf_s( buff, "CHAT from Player[%s]: %s \n", inPacket.m_Name, inPacket.m_Chat );

		Log( "%s \n", buff );
	}
	else
	{
		assert( false );
	}
}