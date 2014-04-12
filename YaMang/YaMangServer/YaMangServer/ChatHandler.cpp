#include "stdafx.h"
#include "ChatHandler.h"
#include "ClientManager.h"


ChatHandler::ChatHandler()
{
}


ChatHandler::~ChatHandler()
{
}

void ChatHandler::HandleEvent( ClientSession* session, PacketHeader& pktBase )
{
	ChatBroadcastRequest inPacket = static_cast<ChatBroadcastRequest&>( pktBase );

	session->m_RecvBuffer.Read( (char*)&inPacket, inPacket.m_Size );

	ChatBroadcastResult outPacket;
	outPacket.m_PlayerId = inPacket.m_PlayerId;
	strcpy_s( outPacket.m_Name, session->m_PlayerName );
	strcpy_s( outPacket.m_Chat, inPacket.m_Chat );

	printf_s( "[%d]%s \n", inPacket.m_PlayerId, inPacket.m_Chat );

	/// 채팅은 바로 방송 하면 끝
	if ( !session->Broadcast( &outPacket ) )
	{
		session->Disconnect();
	}

	// test code
	if ( inPacket.m_Chat[0] == '!')
	{
		if ( strlen( inPacket.m_Chat ) < 4 )
		{
			return;
		}
		std::string directMessage;
		directMessage.append( inPacket.m_Chat );

		int pid = stoi(directMessage.substr( 1, 4 ));
		char msg[1024] = "YOU OUT!";
		strcpy_s( outPacket.m_Chat, msg );
		g_ClientManager->DirectPacket( pid, &outPacket );
	}
	
}
extern ClientManager* g_ClientManager;// also TEST