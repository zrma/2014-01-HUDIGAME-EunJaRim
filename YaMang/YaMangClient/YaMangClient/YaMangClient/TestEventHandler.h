#pragma once
#include "EventHandler.h"

class TestEventHandler: public EventHandler
{
public:
	TestEventHandler();
	~TestEventHandler();

	void HandleEvent( PacketHeader* header, CircularBuffer* buffer );
};

