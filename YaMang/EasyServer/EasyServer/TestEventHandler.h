#pragma once
#include "EventHandler.h"

class TestEventHandler: public EventHandler
{
public:
	TestEventHandler();
	~TestEventHandler();

	void HandleEvent( ClientSession* client, PacketHeader* header, CircularBuffer* buffer, SOCKET* socket );
};

