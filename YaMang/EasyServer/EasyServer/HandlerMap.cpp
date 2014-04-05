#include "stdafx.h"
#include "HandlerMap.h"
#include "TestHandler.h"


HandlerMap::HandlerMap()
{
	/// 패킷 핸들링
	mPacketHandler[PKT_CS_LOGIN] = ClientLoginPacket;
	mPacketHandler[PKT_CS_CHAT] = ClientChatPacket;
}


HandlerMap::~HandlerMap()
{
}

bool HandlerMap::HandleEvent( short* packetType, ClientSession* client, PacketHeader* header, CircularBuffer* buffer, SOCKET* socket )
{
	if ( mPacketHandler[*packetType] )
	{
		mPacketHandler[*packetType]( client, header, buffer, socket );
		return true;
	}
	
	return false;
	
}
