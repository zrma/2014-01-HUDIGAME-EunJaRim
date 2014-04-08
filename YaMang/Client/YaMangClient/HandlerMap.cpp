#include "stdafx.h"
#include "../../PacketType.h"
#include "EventHandler.h"
#include "HandlerMap.h"
#include "LoginEventHandler.h"
#include "ChatEventHandler.h"
#include "TestEventHandler.h"

HandlerMap::HandlerMap()
{
	ZeroMemory( &m_PacketHandler, sizeof( m_PacketHandler ) );
	m_PacketHandler[PKT_SC_LOGIN] = new LoginEventHandler();
	m_PacketHandler[PKT_SC_CHAT] = new ChatEventHandler();
	m_PacketHandler[PKT_SC_TEST] = new TestEventHandler();
}


HandlerMap::~HandlerMap()
{
}

bool HandlerMap::HandleEvent( short* packetType, PacketHeader* header, CircularBuffer* buffer )
{
	if ( m_PacketHandler[*packetType] )
	{
		m_PacketHandler[*packetType]->HandleEvent( header, buffer );
		return true;
	}
	
	return false;
}
