#include "stdafx.h"
#include "TestEventHandler.h"
#include "../../../PacketType.h"
#include "CircularBuffer.h"

TestEventHandler::TestEventHandler()
{
}


TestEventHandler::~TestEventHandler()
{
}

void TestEventHandler::HandleEvent( PacketHeader* header, CircularBuffer* buffer )
{
	/*
	ChatBroadcastRequest inPacket;
	buffer->Read( (char*)&inPacket, header->mSize );
	printf_s( "TEST RECV:%s \n", inPacket.mChat );

	ChatBroadcastResult outPacket;
	outPacket.mPlayerId = inPacket.mPlayerId;
	strcpy_s( outPacket.mName, client->GetPlayerName() );
	strcpy_s( outPacket.mChat, inPacket.mChat );

	/// 채팅은 바로 방송 하면 끝
	if ( !client->Broadcast( &outPacket ) )
		return;
	*/
}
