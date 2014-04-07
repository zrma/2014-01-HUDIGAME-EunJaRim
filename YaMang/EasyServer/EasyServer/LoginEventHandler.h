#pragma once
#include "EventHandler.h"

class LoginEventHandler: public EventHandler
{
public:
	LoginEventHandler();
	~LoginEventHandler();

	void HandleEvent( ClientSession* client, PacketHeader* header, CircularBuffer* buffer, SOCKET* socket );
};

