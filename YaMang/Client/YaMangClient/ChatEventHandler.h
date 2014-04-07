#pragma once
#include "EventHandler.h"

class ChatEventHandler: public EventHandler
{
public:
	ChatEventHandler();
	~ChatEventHandler();

	void HandleEvent( PacketHeader* header, CircularBuffer* buffer );
};

