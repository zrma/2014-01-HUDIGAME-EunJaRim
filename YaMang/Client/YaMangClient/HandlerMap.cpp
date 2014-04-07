#include "stdafx.h"
#include "../../PacketType.h"
#include "EventHandler.h"
#include "HandlerMap.h"
#include "LoginEventHandler.h"
#include "ChatEventHandler.h"
#include "TestEventHandler.h"

HandlerMap::HandlerMap()
{
	// 서버와 여기가 다름
	mPacketHandler[PKT_SC_LOGIN] = new LoginEventHandler();
	mPacketHandler[PKT_SC_CHAT] = new ChatEventHandler();
	mPacketHandler[PKT_SC_TEST] = new TestEventHandler();
}


HandlerMap::~HandlerMap()
{
}

bool HandlerMap::HandleEvent( short* packetType, PacketHeader* header, CircularBuffer* buffer )
{
	if ( mPacketHandler[*packetType] )
	{
		mPacketHandler[*packetType]->HandleEvent( header, buffer );
		return true;
	}
	
	return false;
}
