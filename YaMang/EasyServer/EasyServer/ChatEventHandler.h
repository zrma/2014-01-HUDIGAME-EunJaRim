#pragma once
#include "EventHandler.h"

class ChatEventHandler: public EventHandler
{
public:
	ChatEventHandler();
	~ChatEventHandler();

	void HandleEvent( ClientSession* client, PacketHeader* header, CircularBuffer* buffer, SOCKET* socket );
};

