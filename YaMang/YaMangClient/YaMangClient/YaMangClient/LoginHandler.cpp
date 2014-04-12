#include "stdafx.h"
#include "LoginHandler.h"
#include "NetworkManager.h"


LoginHandler::LoginHandler()
{
}


LoginHandler::~LoginHandler()
{
}

void LoginHandler::HandleEvent( PacketHeader& pktBase )
{
	LoginResult recvData = static_cast<LoginResult&>( pktBase );
	if ( NetworkManager::GetInstance( )->m_RecvBuffer.Read( (char*)&recvData, recvData.m_Size ) )
	{
		// 패킷처리
		if ( recvData.m_PlayerId == -1 )
		{
			/// 여기 걸리면 로그인 실패다.
			ExitProcess( -1 );
		}

		printf_s( "My playerID[%d] \n", recvData.m_PlayerId );
		NetworkManager::GetInstance()->m_MyPlayerId = recvData.m_PlayerId;

		// 				/// 채팅 방송 패킷 보내는 타이머 돌리자.. 
		// 				SetTimer( hWnd, 337, 3000, NULL );
	}
	else
	{
		assert( false );
	}
}
