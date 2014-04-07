#include "stdafx.h"
#include "LoginEventHandler.h"
#include "DatabaseJobContext.h"
#include "DatabaseJobManager.h"
#include "../../PacketType.h"
#include "CircularBuffer.h"
#include "ClientSession.h"

LoginEventHandler::LoginEventHandler()
{
}


LoginEventHandler::~LoginEventHandler()
{
}

void LoginEventHandler::HandleEvent( ClientSession* client, PacketHeader* header, CircularBuffer* buffer, SOCKET* socket )
{
	LoginRequest inPacket;
	buffer->Read( (char*)&inPacket, header->mSize );

	LoadPlayerDataContext* newDbJob = new LoadPlayerDataContext( *socket, inPacket.mPlayerId );
	GDatabaseJobManager->PushDatabaseJobRequest( newDbJob );

}