#include "stdafx.h"
#include "ClientSession.h"
#include "MacroSet.h"

#include "GenerateCorpAction.h"
#include "GameRoom.h"
#include "RoomManager.h"
#include "DatabaseJobContext.h"
#include "DatabaseJobManager.h"

// 로그인을 시도해서 유저 데이터를 불러오면, 데이터를 읽어서 클라이언트 쪽에 Send()
void ClientSession::LoginDone( int pid, const char* name )
{
	g_PidSessionTable.insert( std::pair<int, ClientSession*>( pid, this ) );

	LoginResult outPacket;

	outPacket.m_PlayerId = m_PlayerID = pid;
	strcpy_s( m_PlayerName, name );
	strcpy_s( outPacket.m_Name, name );

	SendRequest( &outPacket );

	m_Logon = true;
}


void ClientSession::DatabaseJobDone( DatabaseJobContext* result )
{
	CRASH_ASSERT( m_Socket == result->m_SockKey );


	const type_info& typeInfo = typeid( *result );

	if ( typeInfo == typeid( LoadPlayerDataContext ) )
	{
		LoadPlayerDataContext* login = dynamic_cast<LoadPlayerDataContext*>( result );

		LogD( "[DEBUG][%d] Login DONE! \n", login->m_PlayerId );
		LoginDone( login->m_PlayerId, login->m_PlayerName );

	}
	else if ( typeInfo == typeid( UpdatePlayerDataContext ) )
	{
		UpdateDone();
	}
	else
	{
		CRASH_ASSERT( false );
	}

}

void ClientSession::UpdateDone()
{
	/// 콘텐츠를 넣기 전까지는 딱히 해줄 것이 없다. 단지 테스트를 위해서..
	printf( "DEBUG: Player[%d] Update Done\n", m_PlayerID );
}






void ClientSession::GameStart()
{
	m_GameStarted = true;
	CalculateRegenTime();

	GenerateCorpAction * action = new GenerateCorpAction();
	action->SetGameRoom( m_GameRoom );
	action->SetPlayerID( m_PlayerID );
	action->SetClientSession( this );

	m_GameRoom->AddActionToScheduler( action, 0 );
}

void ClientSession::OnTick()
{
	/// 클라별로 주기적으로 해야될 일은 여기에

	/// 특정 주기로 DB에 위치 저장
	// 	if ( ++m_DbUpdateCount == PLAYER_DB_UPDATE_CYCLE )
	// 	{
	// 		m_DbUpdateCount = 0;
	// 		UpdatePlayerDataContext* updatePlayer = new UpdatePlayerDataContext( m_Socket, m_PlayerId );
	// 
	// 		updatePlayer->m_PosX = m_PosX;
	// 		updatePlayer->m_PosY = m_PosY;
	// 		updatePlayer->m_PosZ = m_PosZ;
	// 		strcpy_s( updatePlayer->m_Comment, "updated_test" ); ///< 일단은 테스트를 위해
	// 		g_DatabaseJobManager->PushDatabaseJobRequest( updatePlayer );
	// 	}

}


void ClientSession::AddCorpsNum()
{
	assert( 0 <= m_CorpsNow );
	++m_CorpsNow;
	CalculateRegenTime();
}

void ClientSession::SubCorpsNum()
{
	assert( 0 < m_CorpsNow );
	--m_CorpsNow;
	CalculateRegenTime();
}

void ClientSession::AddBaseNum()
{
	assert( 0 <= m_BaseNum );
	++m_BaseNum;
	CalculateRegenTime();
}

void ClientSession::SubBaseNum()
{
	assert( 0 < m_BaseNum );
	--m_BaseNum;
	CalculateRegenTime();
}

void ClientSession::CalculateRegenTime()
{
	// 하드 코딩~~ 베이스 하나당 최대 동원수 증가량과 기본값
	m_CorpsMax = ( m_BaseNum * 10 ) + 10;

	if ( m_CorpsMax <= m_CorpsNow )
	{
		m_CorpsRegenTime = INT_MAX;
	}
	else
	{
		// m_CorpsRegenTime = static_cast<ULONGLONG>( sqrt( ( 100 * ( m_CorpsMax + m_CorpsNow ) ) / ( m_CorpsMax - m_CorpsNow ) ) + 30000 );
		// m_CorpsRegenTime = static_cast<ULONGLONG>( ( 1000 * ( m_CorpsMax + m_CorpsNow ) ) / ( m_CorpsMax - m_CorpsNow + 0 ) + 30000 ); // + 0에 보정치를 넣으면 부대수가 많을때 증가하는 타임차를 줄일수 있다.

		// 일단 간단한 식을 하나 만들어두고 테스트를 해 봅시다.
		m_CorpsRegenTime = 30000 + ( m_CorpsNow - m_BaseNum ) * 3000;
	}


	RefreshUIResult outPacket;
	outPacket.m_NowCorpsNum = m_CorpsNow;
	outPacket.m_MaxCorpsNum = m_CorpsMax;
	outPacket.m_BaseNum = m_BaseNum;
	outPacket.m_RegenTime = m_CorpsRegenTime;

	if ( !DirectSend( &outPacket ) )
	{
		// Disconnect();
	}
}





void ClientSession::SetKingIndex( int index, float startPositionX, float startPositionZ )
{
	m_KingIndex = index;

	KingIndexResult outPacket;
	outPacket.m_KingIndex = index;
	outPacket.m_StartPositionX = startPositionX;
	outPacket.m_StartPositionZ = startPositionZ;

	if ( !DirectSend( &outPacket ) )
	{
		// Disconnect();
	}
}





bool ClientSession::Broadcast( PacketHeader* pkt )
{
	if ( !m_GameStarted )
	{
		return false;
	}

	if ( !IsConnected() )
	{
		return false;
	}

	m_GameRoom->BroadcastPacket( pkt );

	return true;
}

bool ClientSession::DirectSend( PacketHeader* pkt )
{
	if ( !m_GameStarted )
	{
		return false;
	}

	if ( !IsConnected() )
	{
		return false;
	}

	if ( m_GameRoom->DirectPacket( m_PlayerID, pkt ) )
	{
		return true;
	}

	return false;

}






void ClientSession::Disconnect()
{
	if ( !IsConnected() )
	{
		return;
	}

	// g_PidSocketTable.erase( m_PlayerId ); 이거 한줄로 될것 같은데 교수님이 find하고 뭐 하라고 했던게 자꾸 생각남
	auto toBeDelete = g_PidSessionTable.find( m_PlayerID );
	if ( toBeDelete != g_PidSessionTable.end() )
	{
		g_PidSessionTable.erase( toBeDelete );
	}

	printf( "[DEBUG] Client Disconnected: IP=%s, PORT=%d\n", inet_ntoa( m_ClientAddr.sin_addr ), ntohs( m_ClientAddr.sin_port ) );

	/// 즉각 해제
	LINGER lingerOption;
	lingerOption.l_onoff = 1;
	lingerOption.l_linger = 0;

	/// no TCP TIME_WAIT
	if ( SOCKET_ERROR == setsockopt( m_Socket, SOL_SOCKET, SO_LINGER, (char*)&lingerOption, sizeof( LINGER ) ) )
	{
		LogD( "[DEBUG] setsockopt linger option error: %d\n", GetLastError() );
		return;
	}

	closesocket( m_Socket );

	m_Connected = false;

	if ( m_GameStarted && m_GameRoom->IsGameRoomStart() )
	{
		m_GameStarted = false;

		m_GameRoom->GameRoomGiveUp();
	}

}
