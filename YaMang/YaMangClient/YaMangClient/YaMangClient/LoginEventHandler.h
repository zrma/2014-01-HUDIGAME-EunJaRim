#pragma once
#include "EventHandler.h"

class LoginEventHandler: public EventHandler
{
public:
	LoginEventHandler();
	~LoginEventHandler();

	void HandleEvent( PacketHeader* header, CircularBuffer* buffer );
};

