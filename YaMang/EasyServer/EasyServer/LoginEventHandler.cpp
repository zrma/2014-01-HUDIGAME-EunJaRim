#include "stdafx.h"
#include "LoginEventHandler.h"
#include "DatabaseJobContext.h"
#include "DatabaseJobManager.h"


LoginEventHandler::LoginEventHandler()
{
}


LoginEventHandler::~LoginEventHandler()
{
}

void LoginEventHandler::ReadData( CircularBuffer* mRecvBuffer, LoginRequest* inPacket, PacketHeader* header )
{
	mRecvBuffer->Read( (char*)inPacket, header->mSize );
}

void LoginEventHandler::HandleEvent( SOCKET* mSocket, LoginRequest* inPacket, PacketHeader* header )
{
	LoadPlayerDataContext* newDbJob = new LoadPlayerDataContext( *mSocket, inPacket->mPlayerId );
	GDatabaseJobManager->PushDatabaseJobRequest( newDbJob );
}
