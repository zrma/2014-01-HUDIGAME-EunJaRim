#include "stdafx.h"
#include "PacketType.h"
#include "NetworkManager.h"
#include "GameManager.h"
#include "Corps.h"
#include "SceneManager.h"
#include "ScenePlay.h"
#include "TextManager.h"
#include "SoundManager.h"

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
			TextManager::GetInstance( )->RegistText( 300, L"I'm WIN!!", 200, 200 );
		}
		else
		{
			Log( "I'm Lose... \n" );
			TextManager::GetInstance( )->RegistText( 300, L"I'm Lose... ", 200, 200 );
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


REGISTER_HANDLER( PKT_SC_GENERATE_CORPS )
{
		GenerateCorpsResult recvData = static_cast<GenerateCorpsResult&>( pktBase );
		NetworkManager::GetInstance( )->HandleGenerateCorpsResult( recvData );
}

void NetworkManager::HandleGenerateCorpsResult( GenerateCorpsResult& inPacket )
{
	if ( m_RecvBuffer.Read( (char*)&inPacket, inPacket.m_Size ) )
	{
		/// 여기 걸리면 로그인 안된놈이 보낸거다
		assert( inPacket.m_PlayerId != -1 );

		UnitType unitType = inPacket.m_UnitType;

		float nowX = inPacket.m_NowX;
		float nowZ = inPacket.m_NowZ;
		float lookX = inPacket.m_LookX;
		float lookZ = inPacket.m_LookZ;

		PositionInfo position;
		position.m_EyePoint = { nowX, 0.0f, nowZ };
		position.m_LookAtPoint = { lookX, 0.0f, lookZ };

		int corpsID = inPacket.m_CorpsID;
		int playerID = inPacket.m_PlayerId;

		Corps* corps = new Corps( corpsID, playerID, position );
		corps->Create( 10, unitType );
		corps->SetVisible( true );

		Scene* scene =  SceneManager::GetInstance()->GetNowScene();
		if ( typeid( ScenePlay ) == typeid( *scene ) )
		{
			ScenePlay* scenePlay = static_cast<ScenePlay*>( SceneManager::GetInstance()->GetNowScene() );
			scenePlay->AddCorps( corpsID, corps );
			Log( "GenerateCorps! Type:%d CorpID:%d \n", unitType, corpsID );

			SoundManager::GetInstance( )->test2( );
		}
		else
		{
			// 플레이중이 아닌데 플레이용 패킷을 받음 
			assert( false );
		}
	}
	else
	{
		assert( false );
	}
}


REGISTER_HANDLER( PKT_SC_CORPS_CHANGE_FORMATION )
{
	ChangeCorpsFormationResult recvData = static_cast<ChangeCorpsFormationResult&>( pktBase );
	NetworkManager::GetInstance()->HandleChangeCorpsFormationResult( recvData );
}

void NetworkManager::HandleChangeCorpsFormationResult( ChangeCorpsFormationResult& inPacket )
{
	if ( m_RecvBuffer.Read( (char*)&inPacket, inPacket.m_Size ) )
	{

		int corpsID = inPacket.m_CorpsID;
		FormationType formationType = inPacket.m_FormationType;

		Scene* scene = SceneManager::GetInstance()->GetNowScene();
		if ( typeid( ScenePlay ) == typeid( *scene ) )
		{
			ScenePlay* scenePlay = static_cast<ScenePlay*>( SceneManager::GetInstance()->GetNowScene() );
			scenePlay->ChangeCorpsFormation( corpsID, formationType );
			Log( "ChangeCorpsFormation! CorpID:%d Type:%d  \n", corpsID, formationType );
		}
		else
		{
			// 플레이중이 아닌데 플레이용 패킷을 받음 
			assert( false );
		}
	}
	else
	{
		assert( false );
	}
}



REGISTER_HANDLER( PKT_SC_MOVE_CORPS )
{
	MoveCorpsResult recvData = static_cast<MoveCorpsResult&>( pktBase );
	NetworkManager::GetInstance( )->HandleMoveCorpsResult( recvData );
}

void NetworkManager::HandleMoveCorpsResult( MoveCorpsResult& inPacket )
{
	if ( m_RecvBuffer.Read( (char*)&inPacket, inPacket.m_Size ) )
	{
		int corpsID = inPacket.m_CorpsID;
		float speed = inPacket.m_Speed;

		float targetX = inPacket.m_TargetX;
		float targetZ = inPacket.m_TargetZ;
		float lookX = inPacket.m_LookX;
		float lookZ = inPacket.m_LookZ;

		PositionInfo direction;
		direction.m_EyePoint = { targetX, 0.0f, targetZ };
		direction.m_LookAtPoint = { lookX, 0.0f, lookZ };

		Scene* scene = SceneManager::GetInstance()->GetNowScene();
		if ( typeid( ScenePlay ) == typeid( *scene ) )
		{
			ScenePlay* scenePlay = static_cast<ScenePlay*>( SceneManager::GetInstance()->GetNowScene() );
			scenePlay->MoveCorpsStart( corpsID, direction.m_LookAtPoint.x, direction.m_LookAtPoint.z, speed );
			Log( "CorpsMoving! CorpID:%d DesX:%f DesZ:%f Speed:%f \n", corpsID, direction.m_LookAtPoint.x, direction.m_LookAtPoint.z, speed );
		}
		else
		{
			// 플레이중이 아닌데 플레이용 패킷을 받음 
			assert( false );
		}
	}
	else
	{
		assert( false );
	}
}


REGISTER_HANDLER( PKT_SC_STOP_CORPS )
{
	StopCorpsResult recvData = static_cast<StopCorpsResult&>( pktBase );
	NetworkManager::GetInstance()->HandleStopCorpsResult( recvData );
}

void NetworkManager::HandleStopCorpsResult( StopCorpsResult& inPacket )
{
	if ( m_RecvBuffer.Read( (char*)&inPacket, inPacket.m_Size ) )
	{

		int corpsID = inPacket.m_CorpsID;

		Scene* scene = SceneManager::GetInstance()->GetNowScene();
		if ( typeid( ScenePlay ) == typeid( *scene ) )
		{
			ScenePlay* scenePlay = static_cast<ScenePlay*>( SceneManager::GetInstance()->GetNowScene() );
			scenePlay->MoveCorpsStop( corpsID );
			Log( "CorpsStop! CorpID:%d \n", corpsID );
		}
		else
		{
			// 플레이중이 아닌데 플레이용 패킷을 받음 
			assert( false );
		}
	}
	else
	{
		assert( false );
	}
}