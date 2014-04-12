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

	session->mRecvBuffer.Read( (char*)&inPacket, inPacket.mSize );

	ChatBroadcastResult outPacket;
	outPacket.mPlayerId = inPacket.mPlayerId;
	strcpy_s( outPacket.mName, session->mPlayerName );
	strcpy_s( outPacket.mChat, inPacket.mChat );

	printf_s( "[%d]%s \n", inPacket.mPlayerId, inPacket.mChat );

	/// 채팅은 바로 방송 하면 끝
	if ( !session->Broadcast( &outPacket ) )
	{
		session->Disconnect();
	}

	// test code
	if ( inPacket.mChat[0] == '!')
	{
		if ( strlen( inPacket.mChat ) < 4 )
		{
			return;
		}
		std::string directMessage;
		directMessage.append( inPacket.mChat );

		int pid = stoi(directMessage.substr( 1, 4 ));
		char msg[1024] = "YOU OUT!";
		strcpy_s( outPacket.mChat, msg );
		g_ClientManager->DirectPacket( pid, &outPacket );
	}
	
}
extern ClientManager* g_ClientManager;// also TEST