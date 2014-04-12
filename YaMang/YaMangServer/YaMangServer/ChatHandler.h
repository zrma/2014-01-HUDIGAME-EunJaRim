#pragma once
#include "ClientSession.h"

class ChatHandler
{
public:
	ChatHandler();
	~ChatHandler();

	static void HandleEvent( ClientSession* session, PacketHeader& pktBase );
};

