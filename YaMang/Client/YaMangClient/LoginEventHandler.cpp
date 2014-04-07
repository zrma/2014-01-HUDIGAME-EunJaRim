#include "stdafx.h"
#include "LoginEventHandler.h"
#include "../../PacketType.h"
#include "CircularBuffer.h"

LoginEventHandler::LoginEventHandler()
{
}


LoginEventHandler::~LoginEventHandler()
{
}

void LoginEventHandler::HandleEvent( PacketHeader* header, CircularBuffer* buffer )
{
	/*
	LoginRequest inPacket;
	buffer->Read( (char*)&inPacket, header->mSize );

	LoadPlayerDataContext* newDbJob = new LoadPlayerDataContext( *socket, inPacket.mPlayerId );
	GDatabaseJobManager->PushDatabaseJobRequest( newDbJob );
	*/
}