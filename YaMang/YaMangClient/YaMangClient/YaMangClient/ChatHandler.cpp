#include "stdafx.h"
#include "ChatHandler.h"
#include "NetworkManager.h"

extern float myPosX;
extern float myPosY;
extern float myPosZ;

ChatHandler::ChatHandler()
{
}


ChatHandler::~ChatHandler()
{
}

void ChatHandler::HandleEvent( PacketHeader& pktBase )
{
	ChatBroadcastResult recvData = static_cast<ChatBroadcastResult&>( pktBase );
	
	if ( NetworkManager::GetInstance( )->m_RecvBuffer.Read( (char*)&recvData, recvData.m_Size ) )
	{
		/// 여기 걸리면 로그인 안된놈이 보낸거다
		assert( recvData.m_PlayerId != -1 );

		char buff[MAX_CHAT_LEN] = { 0, };
		sprintf_s( buff, "CHAT from Player[%s]: %s \n", recvData.m_Name, recvData.m_Chat );

		if ( recvData.m_Chat[0] == 'L' )
		{
			myPosX -= 2.0f;
		}
		if ( recvData.m_Chat[0] == 'R' )
		{
			myPosX += 2.0f;
		}

		Log( "%s \n", buff );
	}
	else
	{
		assert( false );
	}
}
