#include "stdafx.h"
#include "PacketType.h"
#include "ChatHandler.h"
#include "NetworkManager.h"
#include "MainWindow.h"
#include "KeyInput.h"

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

		if ( recvData.m_Chat[0] == 'A' )
		{
			PostMessage( MainWindow::GetInstance()->Window(), WM_KEYDOWN, VK_A, NULL );
		}
		if ( recvData.m_Chat[0] == 'D' )
		{
			PostMessage( MainWindow::GetInstance()->Window(), WM_KEYDOWN, VK_D, NULL );
		}
		if ( recvData.m_Chat[0] == 'W' )
		{
			PostMessage( MainWindow::GetInstance()->Window(), WM_KEYDOWN, VK_W, NULL );
		}
		if ( recvData.m_Chat[0] == 'S' )
		{
			PostMessage( MainWindow::GetInstance()->Window(), WM_KEYDOWN, VK_S, NULL );
		}
		if ( recvData.m_Chat[0] == 'F' )
		{
			PostMessage( MainWindow::GetInstance()->Window(), WM_KEYUP, VK_A, NULL );
			PostMessage( MainWindow::GetInstance()->Window(), WM_KEYUP, VK_D, NULL );
			PostMessage( MainWindow::GetInstance()->Window(), WM_KEYUP, VK_W, NULL );
			PostMessage( MainWindow::GetInstance()->Window(), WM_KEYUP, VK_S, NULL );
		}

		Log( "%s \n", buff );
	}
	else
	{
		assert( false );
	}
}
