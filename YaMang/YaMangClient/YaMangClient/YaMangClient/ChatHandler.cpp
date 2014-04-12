#include "stdafx.h"
#include "ChatHandler.h"
#include "NetworkManager.h"

extern float posX;
extern float posY;
extern float posZ;

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
			posX -= 2.0f;
		}
		if ( recvData.m_Chat[0] == 'R' )
		{
			posX += 2.0f;
		}

		// 				static int y2pos = 60;
		// 				HDC hdc = GetDC( hWnd );
		// 				TextOutA( hdc, 10, y2pos, buff, strlen( buff ) );
		// 				ReleaseDC( hWnd, hdc );
		// 				y2pos += 15;
		// 				if ( y2pos > 600 )
		// 					y2pos = 60;
		// 
		Log( "%s \n", buff );
	}
	else
	{
		assert( false );
	}
}
