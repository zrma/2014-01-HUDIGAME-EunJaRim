#include "stdafx.h"
#include "ClientSession.h"
#include "PacketType.h"
#include "DatabaseJobContext.h"
#include "DatabaseJobManager.h"

//////////////////////////////////////////////////////////////////////////
#include "RoomManager.h"
#include "GameRoom.h"
#include "Corps.h"
#include "MovePosition.h"
#include "SharedDefine.h"
#include "Attack.h"
#include "TakeArea.h"
#include "MacroSet.h"
#include "KnightAttack.h"

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
		Log( "[Disconnected from:]ClientSession::HandleChatRequest \n" );
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

	// GM이 아니면 아웃
	if ( playerID < 101 && playerID > 199 )
	{
		Log( "[Disconnected from:]ClientSession::HandleGameOverRequest GM out \n" );
		Disconnect( );
		return;
	}


	GameOverResult outPacket;
	outPacket.m_PlayerId = playerID;

	outPacket.m_IsWon = isWon;

	Log( "[GameOverMessage][%d]%d \n", inPacket.m_PlayerId, isWon );

	/// 채팅은 바로 방송 하면 끝
	if ( !Broadcast( &outPacket ) )
	{
		Log( "[Disconnected from:]ClientSession::HandleGameOverRequest Broadcast Fail \n" );
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
		Log( "[Disconnected from:]ClientSession::HandleRoomCreateRequest \n" );
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

	g_RoomManager->AddRoom( );

	if ( !g_RoomManager->CheckRoom( roomNumber ) )
	{
		++m_ErrorNumber;
		return;
	}
	
	if ( !g_RoomManager->EnterRoom( roomNumber, m_PlayerID ) )
	{
		Log( "[Disconnected from:]ClientSession::HandleEnterRoomRequest Enter Room \n" );
		Disconnect();
	}

	EnterRoomResult outPacket;
	outPacket.m_RoomNumber = roomNumber;

	if ( !Broadcast( &outPacket ) )
	{
		//Log( "[Disconnected from:]ClientSession::HandleEnterRoomRequest Broadcast Fail \n" );
		//Disconnect();
	}

	Log( "Enter Room! ID:%d ROOM:%d \n", m_PlayerID, roomNumber );
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

	if ( !g_RoomManager->CheckRoom( roomNumber ) )
	{
		++m_ErrorNumber;
		return;
	}

	if ( !g_RoomManager->LeaveRoom( roomNumber, m_PlayerID ) )
	{
		Log( "[Disconnected from:]ClientSession::HandleLeaveRoomRequest LeaveRoom Error \n" );
		Disconnect();
	}

	EnterRoomResult outPacket;
	outPacket.m_RoomNumber = roomNumber;

	if ( !Broadcast( &outPacket ) )
	{
		Log( "[Disconnected from:]ClientSession::HandleLeaveRoomRequest Broadcast Fail \n" );
		Disconnect();
	}

	Log( "Leave Room! ID:%d ROOM:%d \n", m_PlayerID, roomNumber );
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

	float nowX = inPacket.m_NowX;
	float nowZ = inPacket.m_NowZ;
	float lookX = inPacket.m_LookX;
	float lookZ = inPacket.m_LookZ;
	int playerID = inPacket.m_PlayerId;

	// gm만 리퀘스트를 할수있게... 치트 테스트를 위해 잠시 꺼둠
// 	if ( playerID < 101 && playerID > 199 )
// 	{
// 		Disconnect();
// 		return;
// 	}

	PositionInfo position;
	position.m_EyePoint = { nowX, 0.0f, nowZ };
	position.m_LookAtPoint = { lookX, 0.0f, lookZ };

	const Corps* corps = m_ClientManager->GenerateCorps( playerID, unitType, position );

	if ( nullptr == corps )
	{
		return;
	}

	GenerateCorpsResult outPacket;
	outPacket.m_UnitType = unitType;
	outPacket.m_NowX = nowX;
	outPacket.m_NowZ = nowZ;
	outPacket.m_LookX = lookX;
	outPacket.m_LookZ = lookZ;
	outPacket.m_CorpsID = corps->GetCorpsID();
	outPacket.m_PlayerId = corps->GetPlayerID( );
	outPacket.m_FormationType = corps->GetFormationType();
	outPacket.m_UnitNum = corps->GetUnitNum();

	if ( !Broadcast( &outPacket ) )
	{
		Log( "[Disconnected from:]ClientSession::HandleGenerateCorpsRequest Broadcast Fail \n" );
		Disconnect();
	}

	Log( "GenerateCorps! Type:%d CorpID:%d PlayerID:%d \n",
			  unitType, corps->GetCorpsID( ), m_PlayerID );
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

	float nowX = inPacket.m_NowX;
	float nowZ = inPacket.m_NowZ;
	float targetX = inPacket.m_TargetX;
	float targetZ = inPacket.m_TargetZ;

	PositionInfo destination;
	destination.m_EyePoint = { nowX, 0.0f, nowZ };
	destination.m_LookAtPoint = { targetX, 0.0f, targetZ };

	Corps* corps = m_ClientManager->GetCorpsByCorpsID( corpsID );

	if ( nullptr == corps )
	{
		++m_ErrorNumber;
		return;
	}

	// 가드나 킹이 이동 요청해옴
	if ( UnitType::UNIT_GUARD == corps->GetUnitType( ) || UnitType::UNIT_KING == corps->GetUnitType( ) )
	{
		Log( "[Error] [%d]Type Move Request \n", corpsID );
		++m_ErrorNumber;
		return;
	}

	if ( m_ErrorNumber > m_ErrorNumberMax )
	{
		Log( "[Disconnected from:]ClientSession::HandleMoveCorpsRequest Error MAX \n" );
		Disconnect();
	}

	// Corps를 탐색 한 후 Corps의 정체를 파악
	// -- 적합한 이동인지 판정 후 스케쥴러에 등록
	//
	// 이후 스케쥴러는 해당 타이밍 마다 액션을 꺼내서 수행
	// 이동 액션은 내부적으로 Corps의 상황을 파악하여
	// 방향과 속도 등의 정보를 담아서 타이머 발생 때 마다 Result Packet을 보낸다.

	
	std::shared_ptr<MovePosition>action( new MovePosition );
	
	action->SetClientManager( m_ClientManager );
	action->SetOwnerCorps( corps );
	action->SetDestination( destination );

	m_ClientManager->AddActionToScheduler( action, 0 );

	Log( "CorpsMoved CorpID:%d PlayerID:%d DesX:%f DesZ:%f \n", corpsID, m_PlayerID, destination.m_EyePoint.x, destination.m_EyePoint.z );

}


REGISTER_HANDLER( PKT_CS_STOP_CORPS )
{
	StopCorpsRequest inPacket = static_cast<StopCorpsRequest&>( pktBase );
	session->HandleStopCorpsRequest( inPacket );
}

void ClientSession::HandleStopCorpsRequest( StopCorpsRequest& inPacket )
{
	m_RecvBuffer.Read( (char*)&inPacket, inPacket.m_Size );

	int corpsID = inPacket.m_CorpsID;

	if ( nullptr == m_ClientManager->GetCorpsByCorpsID( corpsID ) )
	{
		++m_ErrorNumber;
		return;
	}

	if ( m_ErrorNumber > m_ErrorNumberMax )
	{
		Log( "[Disconnected from:]ClientSession::HandleStopCorpsRequest Error MAX \n" );
		Disconnect();
	}

	// MOVE!!!!!!;
	// 미구현 그냥 클라쪽 패킷만 일단 구현

	StopCorpsResult outPacket;
	outPacket.m_CorpsID = corpsID;

	if ( !Broadcast( &outPacket ) )
	{
		Log( "[Disconnected from:]ClientSession::HandleStopCorpsRequest BroadCast fail \n" );
		Disconnect();
	}

	Log( "CorpsStopped! CorpID:%d PlayerID:%d \n", corpsID, m_PlayerID );

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

	if ( nullptr == m_ClientManager->GetCorpsByCorpsID( corpsID ) )
	{
		++m_ErrorNumber;
		return;
	}

	if ( m_ErrorNumber > m_ErrorNumberMax )
	{
		Log( "[Disconnected from:]ClientSession::HandleChangeCorpsFormationRequest Error Max \n" );
		Disconnect();
	}

	// 내 콥스 맵에서 포메이션바꿔주자

	ChangeCorpsFormationResult outPacket;
	outPacket.m_CorpsID = corpsID;
	outPacket.m_FormationType = formation;

	if ( !Broadcast( &outPacket ) )
	{
		Log( "[Disconnected from:]ClientSession::HandleChangeCorpsFormationRequest BoradCast Fail \n" );
		Disconnect();
	}

	Log( "Corps Change Formation CorpID:%d Formation:%d \n", corpsID, formation );

}





REGISTER_HANDLER( PKT_CS_CORPS_ATTACK )
{
	AttackCorpsRequest inPacket = static_cast<AttackCorpsRequest&>( pktBase );
	session->HandleAttackCorpsRequest( inPacket );
}

void ClientSession::HandleAttackCorpsRequest( AttackCorpsRequest& inPacket )
{

	m_RecvBuffer.Read( (char*)&inPacket, inPacket.m_Size );


	int myCorpsID = inPacket.m_MyCorpsID;
	int targetCorpsID = inPacket.m_TargetCorpsID;

	Corps* myCorps = m_ClientManager->GetCorpsByCorpsID( myCorpsID );
	Corps* targetCorps = m_ClientManager->GetCorpsByCorpsID( targetCorpsID );

	if ( nullptr == myCorps || nullptr == targetCorps )
	{
		++m_ErrorNumber;
		return;
	}

	if ( myCorps->GetPlayerID() == targetCorps->GetPlayerID() )
	{
		Log( "[BUG]Attack Own Corps! \n" );
		++m_ErrorNumber;
		return;
	}

	if ( m_ErrorNumber > m_ErrorNumberMax )
	{
		Log( "[Disconnected from:]ClientSession::HandleAttackCorpsRequest Error Max \n" );
		Disconnect();
	}

	UnitType unitType = targetCorps->GetUnitType();
	if ( UnitType::UNIT_GUARD == unitType || UnitType::UNIT_KING == unitType )
	{
		std::shared_ptr<TakeArea>action( new TakeArea );
		action->SetClientManager( m_ClientManager );
		action->SetOwnerCorps( myCorps );
		action->SetTargetCorps( targetCorps );

		m_ClientManager->AddActionToScheduler( action, 0 );
		Log( "[Packet GET]TakeBase FromCorpID:%d ToCorpID:%d \n", myCorpsID, targetCorpsID );
	}
	else
	{
		if ( UnitType::UNIT_KNIGHT == unitType )
		{
			std::shared_ptr<KnightAttack>action( new KnightAttack );
			action->SetClientManager( m_ClientManager );
			action->SetOwnerCorps( myCorps );
			action->SetTargetCorps( targetCorps );

			m_ClientManager->AddActionToScheduler( action, 0 );
			Log( "[Packet GET]KnightAttackCorps FromCorpID:%d ToCorpID:%d \n", myCorpsID, targetCorpsID );
		}
		else
		{
			std::shared_ptr<Attack>action( new Attack );
			action->SetClientManager( m_ClientManager );
			action->SetOwnerCorps( myCorps );
			action->SetTargetCorps( targetCorps );

			m_ClientManager->AddActionToScheduler( action, 0 );
			Log( "[Packet GET]AttackCorps FromCorpID:%d ToCorpID:%d \n", myCorpsID, targetCorpsID );
		}

	}


	
}




REGISTER_HANDLER( PKT_CS_SYNC_ALL )
{
	SyncAllRequest inPacket = static_cast<SyncAllRequest&>( pktBase );
	session->HandleSyncAllRequest( inPacket );
}

void ClientSession::HandleSyncAllRequest( SyncAllRequest& inPacket )
{

	m_RecvBuffer.Read( (char*)&inPacket, inPacket.m_Size );

	int roomNumber = inPacket.m_RoomNumber;
	int playerID = inPacket.m_PlayerId;

	const std::map<int, Corps*> corpsList = m_ClientManager->GetCorpsList( );

	for ( auto& it : corpsList )
	{
		Corps* corps = it.second;

		if ( corps->IsDead() )
		{
			continue;
		}
		const PositionInfo& positionInfo = corps->GetPositionInfo( );

		GenerateCorpsResult outPacket;
		outPacket.m_UnitType = corps->GetUnitType( );
		outPacket.m_NowX = positionInfo.m_EyePoint.x;
		outPacket.m_NowZ = positionInfo.m_EyePoint.z;
		outPacket.m_LookX = positionInfo.m_LookAtPoint.x;
		outPacket.m_LookZ = positionInfo.m_LookAtPoint.z;
		outPacket.m_CorpsID = corps->GetCorpsID( );
		outPacket.m_PlayerId = corps->GetPlayerID( );
		outPacket.m_FormationType = corps->GetFormationType( );
		outPacket.m_UnitNum = corps->GetUnitNum( );


		if ( !DirectSend( &outPacket ) )
		{
			Log( "[Disconnected from:]ClientSession::HandleSyncAllRequest DirectSend Fail \n" );
			Disconnect();
		}
	}




}

//////////////////////////////////////////////////////////////////////////
