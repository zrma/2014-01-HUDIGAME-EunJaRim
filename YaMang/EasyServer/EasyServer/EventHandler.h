#pragma once
#include "ClientSession.h"
class EventHandler
{
public:
	virtual void HandleEvent( ClientSession* client, PacketHeader* header, CircularBuffer* buffer, SOCKET* socket ) = 0;
};