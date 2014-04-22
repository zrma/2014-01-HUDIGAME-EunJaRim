#include "stdafx.h"
#include "ClientSession.h"
#include "PacketType.h"
#include "DatabaseJobContext.h"
#include "DatabaseJobManager.h"

//////////////////////////////////////////////////////////////////////////
#include "RoomManager.h"
#include "ClientManager.h"
#include "Corps.h"

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

	std::string packetMessage;
	packetMessage.append( inPacket.m_Chat );

	//////////////////////////////////////////////////////////////////////////
	// 테스트용으로 임시로 붙여둠
	//////////////////////////////////////////////////////////////////////////
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




REGISTER_HANDLER( PKT_CS_ROOM_CHANGE )
{
	RoomChangeRequest inPacket = static_cast<RoomChangeRequest&>( pktBase );
	session->HandleRoomChangeRequest( inPacket );
}

void ClientSession::HandleRoomChangeRequest( RoomChangeRequest& inPacket )
{

	m_RecvBuffer.Read( (char*)&inPacket, inPacket.m_Size );

	int pid = inPacket.m_PlayerId;

	int roomFrom = inPacket.m_RoomFrom;
	int roomTo = inPacket.m_RoomTo;

	if ( !g_RoomManager->ChangeRoom( roomFrom, roomTo, pid ) )
	{
		Disconnect();
	}

	RoomChangeResult outPacket;
	outPacket.m_RoomNumber = roomTo;

	if ( !Broadcast( &outPacket ) )
	{
		Disconnect();
	}

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

	int generatedCorpsID = GenerateCorps( unitType, position );

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

	printf_s( "GenerateCorps! Type:%d CorpID:%d PlayerID:%d CorpsListSize:%d \n", unitType, generatedCorpsID, m_PlayerId, static_cast<int>( m_CorpsList.size() ) );

}





REGISTER_HANDLER( PKT_CS_MOVE_CORPS )
{
	MoveCorpsRequest inPacket = static_cast<MoveCorpsRequest&>( pktBase );
	session->HandleMoveCorpsRequest( inPacket );
}

void ClientSession::HandleMoveCorpsRequest( MoveCorpsRequest& inPacket )
{

	m_RecvBuffer.Read( (char*)&inPacket, inPacket.m_Size );

	int corpsID = inPacket.m_CorpsID;
	Position position = inPacket.m_Position;

	if ( corpsID == -1 )
	{
		Disconnect();
	}

	// MOVE!!!!!!;
	// 미구현

	MoveCorpsResult outPacket;
	outPacket.m_PlayerId = m_PlayerId;
	outPacket.m_CorpsID = corpsID; // 미구현
	outPacket.m_Position = position;
	outPacket.m_CorpsID = -1; // 미구현



	if ( !Broadcast( &outPacket ) )
	{
		Disconnect();
	}

	printf_s( "CorpsMoved CorpID:%d PlayerID:%d PosX:%f PosZ:%f \n", corpsID, m_PlayerId, position.m_EyePoint.x, position.m_EyePoint.y );

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