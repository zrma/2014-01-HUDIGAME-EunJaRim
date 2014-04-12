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
	session->m_RecvBuffer.Read( (char*)&inPacket, inPacket.m_Size );

	/// 로그인은 DB 작업을 거쳐야 하기 때문에 DB 작업 요청한다.
	LoadPlayerDataContext* newDbJob = new LoadPlayerDataContext( session->m_Socket, inPacket.m_PlayerId );
	g_DatabaseJobManager->PushDatabaseJobRequest( newDbJob );
}


