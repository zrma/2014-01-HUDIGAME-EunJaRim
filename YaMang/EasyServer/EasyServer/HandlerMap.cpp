#include "stdafx.h"
#include "HandlerMap.h"
#include "LoginEventHandler.h"
#include "ChatEventHandler.h"
#include "TestEventHandler.h"


HandlerMap::HandlerMap()
{
	/// 패킷 핸들링
	mPacketHandler[PKT_CS_LOGIN] = new LoginEventHandler();
	mPacketHandler[PKT_CS_CHAT] = new ChatEventHandler();
	mPacketHandler[PKT_CS_TEST] = new TestEventHandler();
}


HandlerMap::~HandlerMap()
{
}

bool HandlerMap::HandleEvent( short* packetType, ClientSession* client, PacketHeader* header, CircularBuffer* buffer, SOCKET* socket )
{
	if ( mPacketHandler[*packetType] )
	{
		mPacketHandler[*packetType]->HandleEvent( client, header, buffer, socket );
		return true;
	}
	
	return false;
	
}
