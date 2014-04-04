#include "stdafx.h"

#include "TestHandler.h"
#include "ClientSession.h"
#include "DatabaseJobContext.h"
#include "DatabaseJobManager.h"

void ClientLoginPacket( ClientSession* client, PacketHeader* header, CircularBuffer* buffer, SOCKET* socket)
{
	LoginRequest inPacket;
	buffer->Read( (char*)&inPacket, header->mSize );
	
	LoadPlayerDataContext* newDbJob = new LoadPlayerDataContext( *socket, inPacket.mPlayerId );
	GDatabaseJobManager->PushDatabaseJobRequest( newDbJob );

}
void ClientChatPacket( ClientSession* client, PacketHeader* header, CircularBuffer* buffer, SOCKET* socket )
{

	ChatBroadcastRequest inPacket;
	buffer->Read( (char*)&inPacket, header->mSize );
	printf_s( "CHAT RECV:%s \n", inPacket.mChat );
	
	ChatBroadcastResult outPacket;
	outPacket.mPlayerId = inPacket.mPlayerId;
	strcpy_s( outPacket.mName, client->GetPlayerName() );
	strcpy_s( outPacket.mChat, inPacket.mChat );

	/// 채팅은 바로 방송 하면 끝
	if ( !client->Broadcast( &outPacket ) )
		return;

}


