#include "stdafx.h"
#include "ClientSession.h"
#include "PacketType.h"
#include "DatabaseJobContext.h"
#include "DatabaseJobManager.h"

//////////////////////////////////////////////////////////////////////////
#include "RoomManager.h"
#include "ClientManager.h"
#include "Corps.h"
#include "MovePosition.h"

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

void ClientSession::HandleChatRequest( ChatBroadcastRequest& inPacket )
{
	m_RecvBuffer.Read( (char*)&inPacket, inPacket.m_Size );

	ChatBroadcastResult outPacket;
	outPacket.m_PlayerId = inPacket.m_PlayerId;
	strcpy_s( outPacket.m_Name, m_PlayerName );
	strcpy_s( outPacket.m_Chat, inPacket.m_Chat );

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

	bool isWon = inPacket.m_IsWon;
	int playerID = inPacket.m_PlayerId;

	GameOverResult outPacket;
	outPacket.m_PlayerId = playerID;

	outPacket.m_IsWon = isWon;

	printf_s( "[GameOverMessage][%d]%d \n", inPacket.m_PlayerId, isWon );

	/// 채팅은 바로 방송 하면 끝
	if ( !Broadcast( &outPacket ) )
	{
		Disconnect();
	}
}

REGISTER_HANDLER( PKT_CS_ROOM_CREATE )
{
	RoomCreateRequest inPacket = static_cast<RoomCreateRequest&>( pktBase );
	session->HandleRoomCreateRequest( inPacket );
}

void ClientSession::HandleRoomCreateRequest( RoomCreateRequest& inPacket )
{

	m_RecvBuffer.Read( (char*)&inPacket, inPacket.m_Size );

	// 어차피 방 만들고 요청하는 사람을 그방으로 넣어주는게 좋을까?
	int pid = inPacket.m_PlayerId;

	int roomNumber = g_RoomManager->AddRoom();

	RoomCreateResult outPacket;

	outPacket.m_RoomNumber = roomNumber;

	if ( !DirectSend( &outPacket ) )
	{
		Disconnect();
	}


}


REGISTER_HANDLER( PKT_CS_ENTER_ROOM )
{
	EnterRoomRequest inPacket = static_cast<EnterRoomRequest&>( pktBase );
	session->HandleEnterRoomRequest( inPacket );
}

void ClientSession::HandleEnterRoomRequest( EnterRoomRequest& inPacket )
{

	m_RecvBuffer.Read( (char*)&inPacket, inPacket.m_Size );

	int roomNumber = inPacket.m_RoomNumber;

	if ( !g_RoomManager->EnterRoom( roomNumber, m_PlayerId ) )
	{
		Disconnect();
	}

	EnterRoomResult outPacket;
	outPacket.m_RoomNumber = roomNumber;

	if ( !Broadcast( &outPacket ) )
	{
		Disconnect();
	}

	printf_s( "Enter Room! ID:%d ROOM:%d \n", m_PlayerId, roomNumber );
	g_RoomManager->PrintClientList(); // 테스트 프린트

}


REGISTER_HANDLER( PKT_CS_LEAVE_ROOM )
{
	LeaveRoomRequest inPacket = static_cast<LeaveRoomRequest&>( pktBase );
	session->HandleLeaveRoomRequest( inPacket );
}

void ClientSession::HandleLeaveRoomRequest( LeaveRoomRequest& inPacket )
{

	m_RecvBuffer.Read( (char*)&inPacket, inPacket.m_Size );

	int roomNumber = inPacket.m_RoomNumber;

	if ( !g_RoomManager->LeaveRoom( roomNumber, m_PlayerId ) )
	{
		Disconnect();
	}

	EnterRoomResult outPacket;
	outPacket.m_RoomNumber = roomNumber;

	if ( !Broadcast( &outPacket ) )
	{
		Disconnect();
	}

	printf_s( "Leave Room! ID:%d ROOM:%d \n", m_PlayerId, roomNumber );
	g_RoomManager->PrintClientList(); // 테스트 프린트

}


REGISTER_HANDLER( PKT_CS_GENERATE_CORPS )
{
	GenerateCorpsRequest inPacket = static_cast<GenerateCorpsRequest&>( pktBase );
	session->HandleGenerateCorpsRequest( inPacket );
}

void ClientSession::HandleGenerateCorpsRequest( GenerateCorpsRequest& inPacket )
{
	m_RecvBuffer.Read( (char*)&inPacket, inPacket.m_Size );

	UnitType unitType = inPacket.m_UnitType;
	Position position = inPacket.m_Position;
	
	int generatedCorpsID = m_ClientManager->GenerateCorps( m_PlayerId, unitType, position );

	if ( generatedCorpsID == -1 )
	{
		Disconnect();
	}

	GenerateCorpsResult outPacket;
	outPacket.m_UnitType = unitType;
	outPacket.m_Position = position;
	outPacket.m_CorpsID = generatedCorpsID;
	outPacket.m_PlayerId = m_PlayerId;

	if ( !Broadcast( &outPacket ) )
	{
		Disconnect();
	}

	printf_s( "GenerateCorps! Type:%d CorpID:%d PlayerID:%d \n",
			  unitType, generatedCorpsID, m_PlayerId );
}


REGISTER_HANDLER( PKT_CS_MOVE_CORPS )
{
	MoveCorpsRequest inPacket = static_cast<MoveCorpsRequest&>( pktBase );
	session->HandleMoveCorpsRequest( inPacket );
}

void ClientSession::HandleMoveCorpsRequest( MoveCorpsRequest& inPacket )
{
	m_RecvBuffer.Read( (char*)&inPacket, inPacket.m_Size );
	
	int playerID = inPacket.m_PlayerID;
	int corpsID = inPacket.m_CorpsID;
	float speed = inPacket.m_Speed;
	Position destination = inPacket.m_Destination;

	if ( playerID == -1 )
	{
		Disconnect();
	}
	if ( corpsID == -1 )
	{
		Disconnect();
	}

	

	// MOVE!!!!!!;
	// 미구현 그냥 클라쪽 패킷만 일단 구현
	Action* action = new MovePosition();
	m_ClientManager->AddActionToScheduler( action, 3000 );


	MoveCorpsResult outPacket;
	outPacket.m_PlayerID = playerID;
	outPacket.m_CorpsID = corpsID;
	outPacket.m_Speed = speed;
	outPacket.m_Destination = destination;

	if ( !Broadcast( &outPacket ) )
	{
		Disconnect();
	}

	printf_s( "CorpsMoved CorpID:%d PlayerID:%d DesX:%f DesZ:%f Speed:%f \n", corpsID, m_PlayerId, destination.m_EyePoint.x, destination.m_EyePoint.z, speed );

}


REGISTER_HANDLER( PKT_CS_STOP_CORPS )
{
	StopCorpsRequest inPacket = static_cast<StopCorpsRequest&>( pktBase );
	session->HandleStopCorpsRequest( inPacket );
}

void ClientSession::HandleStopCorpsRequest( StopCorpsRequest& inPacket )
{
	m_RecvBuffer.Read( (char*)&inPacket, inPacket.m_Size );

	int playerID = inPacket.m_PlayerID;
	int corpsID = inPacket.m_CorpsID;

	if ( playerID == -1 )
	{
		Disconnect();
	}
	if ( corpsID == -1 )
	{
		Disconnect();
	}

	// MOVE!!!!!!;
	// 미구현 그냥 클라쪽 패킷만 일단 구현

	MoveCorpsResult outPacket;
	outPacket.m_PlayerID = playerID;
	outPacket.m_CorpsID = corpsID;

	if ( !Broadcast( &outPacket ) )
	{
		Disconnect();
	}

	printf_s( "CorpsStopped! CorpID:%d PlayerID:%d \n", corpsID, m_PlayerId );

}


REGISTER_HANDLER( PKT_CS_CORPS_CHANGE_FORMATION )
{
	ChangeCorpsFormationRequest inPacket = static_cast<ChangeCorpsFormationRequest&>( pktBase );
	session->HandleChangeCorpsFormationRequest( inPacket );
}

void ClientSession::HandleChangeCorpsFormationRequest( ChangeCorpsFormationRequest& inPacket )
{

	m_RecvBuffer.Read( (char*)&inPacket, inPacket.m_Size );


	int corpsID = inPacket.m_CorpsID;
	FormationType formation = inPacket.m_FormationType;

	if ( corpsID == -1 )
	{
		Disconnect();
	}

	// 내 콥스 맵에서 포메이션바꿔주자

	ChangeCorpsFormationResult outPacket;
	outPacket.m_CorpsID = corpsID;
	outPacket.m_FormationType = formation;

	if ( !Broadcast( &outPacket ) )
	{
		Disconnect();
	}

	printf_s( "Corps Change Formation CorpID:%d Formation:%d \n", corpsID, formation );

}