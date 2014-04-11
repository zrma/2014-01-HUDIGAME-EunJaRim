#include "stdafx.h"
#include "LoginHandler.h"
#include "DatabaseJobContext.h"
#include "DatabaseJobManager.h"


LoginHandler::LoginHandler()
{
}


LoginHandler::~LoginHandler()
{
}

void LoginHandler::HandleEvent( ClientSession* session, PacketHeader& pktBase )
{
	LoginRequest inPacket = static_cast<LoginRequest&>( pktBase );
	session->mRecvBuffer.Read( (char*)&inPacket, inPacket.mSize );

	/// 로그인은 DB 작업을 거쳐야 하기 때문에 DB 작업 요청한다.
	LoadPlayerDataContext* newDbJob = new LoadPlayerDataContext( session->mSocket, inPacket.mPlayerId );
	g_DatabaseJobManager->PushDatabaseJobRequest( newDbJob );
}


