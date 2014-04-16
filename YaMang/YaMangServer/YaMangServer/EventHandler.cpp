#include "stdafx.h"
#include "ClientSession.h"
#include "PacketType.h"
#include "DatabaseJobContext.h"
#include "DatabaseJobManager.h"

//////////////////////////////////////////////////////////////////////////
#include "RoomManager.h"
#include "ClientManager.h"
// 테스트용 헤더

extern RoomManager* g_RoomManager;
//////////////////////////////////////////////////////////////////////////

HandlerFunc HandlerTable[PKT_MAX];

static void DefaultHandler( ClientSession* session, PacketHeader& pktBase )
{
	assert( false );
	session->Disconnect();
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
	static void Handler_##PKT_TYPE( ClientSession* session, PacketHeader& pktBase ); \
	static RegisterHandler _register_##PKT_TYPE( PKT_TYPE, Handler_##PKT_TYPE ); \
	static void Handler_##PKT_TYPE( ClientSession* session, PacketHeader& pktBase )

REGISTER_HANDLER( PKT_CS_LOGIN )
{
	LoginRequest inPacket = static_cast<LoginRequest&>( pktBase );
	session->HandleLoginRequest( inPacket );
}

void ClientSession::HandleLoginRequest( LoginRequest& inPacket )
{
	m_RecvBuffer.Read( (char*)&inPacket, inPacket.m_Size );

	/// 로그인은 DB 작업을 거쳐야 하기 때문에 DB 작업 요청한다.
	LoadPlayerDataContext* newDbJob = new LoadPlayerDataContext( m_Socket, inPacket.m_PlayerId );
	g_DatabaseJobManager->PushDatabaseJobRequest( newDbJob );
}

REGISTER_HANDLER( PKT_CS_CHAT )
{
	ChatBroadcastRequest inPacket = static_cast<ChatBroadcastRequest&>( pktBase );
	session->HandleChatRequest( inPacket );
}

//////////////////////////////////////////////////////////////////////////
// 임시로 추가 한 부분
//////////////////////////////////////////////////////////////////////////
std::map<int, std::array<int, 4>> g_EventTimeTable;

struct EventData
{
	int				targetId = 0;
	ClientSession*	session = nullptr;
};

void CALLBACK StopEvent( LPVOID lpArg, DWORD dwTimerLowValue, DWORD dwTimerHighValue )
{
	for ( auto& iter : g_EventTimeTable )
	{
		auto key = iter.first;
		auto& value = iter.second;

		for ( int i = 0; i < 4; ++i )
		{
			if ( value[i] > 0 )
			{
				++value[i];
			}
		}

		ChatBroadcastResult inPacket;
		inPacket.m_PlayerId = key;
		strcpy_s( inPacket.m_Name, "MSG" );

		if ( value[0] > 3 )
		{
			value[0] = 0;
			strcpy_s( inPacket.m_Chat, "StopUp" );
			auto it = g_RoomManager->GetFirstClientManager();
			if ( it )
			{
				printf_s( "%d번 클라이언트 Stop! \n", key );
				it->DirectPacket( key, &inPacket );
			}
		}
		if ( value[1] > 3 )
		{
			value[1] = 0;
			strcpy_s( inPacket.m_Chat, "StopDown" );
			auto it = g_RoomManager->GetFirstClientManager();
			if ( it )
			{
				printf_s( "%d번 클라이언트 Stop! \n", key );
				it->DirectPacket( key, &inPacket );
			}
		}
		if ( value[2] > 3 )
		{
			value[2] = 0;
			strcpy_s( inPacket.m_Chat, "StopLeft" );
			auto it = g_RoomManager->GetFirstClientManager();
			if ( it )
			{
				printf_s( "%d번 클라이언트 Stop! \n", key );
				it->DirectPacket( key, &inPacket );
			}
		}
		if ( value[3] > 3 )
		{
			value[3] = 0;
			strcpy_s( inPacket.m_Chat, "StopRight" );
			auto it = g_RoomManager->GetFirstClientManager();
			if ( it )
			{
				printf_s( "%d번 클라이언트 Stop! \n", key );
				it->DirectPacket( key, &inPacket );
			}
		}
	}
}

void ClientSession::HandleChatRequest( ChatBroadcastRequest& inPacket )
{
	m_RecvBuffer.Read( (char*)&inPacket, inPacket.m_Size );

	ChatBroadcastResult outPacket;
	outPacket.m_PlayerId = inPacket.m_PlayerId;
	strcpy_s( outPacket.m_Name, m_PlayerName );
	// strcpy_s( outPacket.m_Chat, inPacket.m_Chat );

	//////////////////////////////////////////////////////////////////////////
	// 임시로 구현 해 둠
	//////////////////////////////////////////////////////////////////////////
	std::string packetMessage;
	packetMessage.append( inPacket.m_Chat );

	if ( g_EventTimeTable.find( inPacket.m_PlayerId ) == g_EventTimeTable.end() )
	{
		ZeroMemory( &(g_EventTimeTable[inPacket.m_PlayerId]), sizeof( std::array<int, 4> ) );
	}

	if ( packetMessage == "IsMoveUpOK")
	{
		if ( g_EventTimeTable[inPacket.m_PlayerId][0] > 0 )
			return;
		g_EventTimeTable[inPacket.m_PlayerId][0] += 1;
		strcpy_s( outPacket.m_Chat, "MoveUp" );
		printf_s( "%d번 클라이언트 Move! \n", inPacket.m_PlayerId );
		SendRequest( &outPacket );
		return;
	}
	else if ( packetMessage == "IsMoveDownOK")
	{
		if ( g_EventTimeTable[inPacket.m_PlayerId][1] > 0 )
			return;
		g_EventTimeTable[inPacket.m_PlayerId][1] += 1;
		strcpy_s( outPacket.m_Chat, "MoveDown" );
		printf_s( "%d번 클라이언트 Move! \n", inPacket.m_PlayerId );
		SendRequest( &outPacket );
		return;
	}
	else if ( packetMessage == "IsMoveLeftOK" )
	{
		if ( g_EventTimeTable[inPacket.m_PlayerId][2] > 0 )
			return;
		g_EventTimeTable[inPacket.m_PlayerId][2] += 1;
		strcpy_s( outPacket.m_Chat, "MoveLeft" );
		printf_s( "%d번 클라이언트 Move! \n", inPacket.m_PlayerId );
		SendRequest( &outPacket );
		return;
	}
	else if ( packetMessage == "IsMoveRightOK")
	{
		if ( g_EventTimeTable[inPacket.m_PlayerId][3] > 0 )
			return;
		g_EventTimeTable[inPacket.m_PlayerId][3] += 1;
		strcpy_s( outPacket.m_Chat, "MoveRight" );
		printf_s( "%d번 클라이언트 Move! \n", inPacket.m_PlayerId );
		SendRequest( &outPacket );
		return;
	}
	else
	{
		strcpy_s( outPacket.m_Chat, inPacket.m_Chat );
	}
	//////////////////////////////////////////////////////////////////////////
	// 이 위까지 임시로 구현한 것임

	/// 채팅은 바로 방송 하면 끝
	if ( !Broadcast( &outPacket ) )
	{
		Disconnect();
	}
}

REGISTER_HANDLER( PKT_CS_GAMEOVER )
{
	GameOverRequest inPacket = static_cast<GameOverRequest&>( pktBase );
	session->HandleGameOverRequest( inPacket );
}

void ClientSession::HandleGameOverRequest( GameOverRequest& inPacket )
{

	m_RecvBuffer.Read( (char*)&inPacket, inPacket.m_Size );

	std::string packetMessage;
	packetMessage.append( inPacket.m_Chat );

	//////////////////////////////////////////////////////////////////////////
	// 테스트용으로 임시로 붙여둠
	//////////////////////////////////////////////////////////////////////////
	try
	{
		int pid = stoi( packetMessage.substr( 1, 4 ) );
		// 예외 상황이 발생 할 수 있음
		//
		// 1) 4글자 미만일 경우 펑!
		// 2) 숫자가 아닐 경우 펑!

		GameOverResult outPacket;
		outPacket.m_PlayerId = pid;

		if ( packetMessage.at( 0 ) == 'W' )
		{
			outPacket.m_IsWon = true;
		}
		else
		{
			outPacket.m_IsWon = false;
		}

		printf_s( "[GameOverMessage][%d]%s \n", inPacket.m_PlayerId, inPacket.m_Chat );

		/// 채팅은 바로 방송 하면 끝
		if ( !Broadcast( &outPacket ) )
		{
			Disconnect();
		}
	}
	catch (...)
	{
		return;
	}
}