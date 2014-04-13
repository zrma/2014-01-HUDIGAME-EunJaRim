#include "stdafx.h"
#include "ClientSession.h"
#include "GameOverHandler.h"


GameOverHandler::GameOverHandler()
{
}


GameOverHandler::~GameOverHandler()
{
}

void GameOverHandler::HandleEvent( ClientSession* session, PacketHeader& pktBase )
{
	GameOverRequest inPacket = static_cast<GameOverRequest&>( pktBase );

	session->m_RecvBuffer.Read( (char*)&inPacket, inPacket.m_Size );

	std::string packetMessage;
	packetMessage.append( inPacket.m_Chat );
	

	GameOverResult outPacket;
	int pid = stoi( packetMessage.substr( 1, 4 ) );
	outPacket.m_PlayerId = pid;

	if ( packetMessage.at(0) == 'W' )
	{
		outPacket.m_IsWon = true;
	}
	else
	{
		outPacket.m_IsWon = false;
	}

	printf_s( "[GameOverMessage][%d]%s \n", inPacket.m_PlayerId, inPacket.m_Chat );

	/// 채팅은 바로 방송 하면 끝
	if ( !session->Broadcast( &outPacket ) )
	{
		session->Disconnect();
	}

}