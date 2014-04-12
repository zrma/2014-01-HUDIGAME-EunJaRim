#pragma once
#include "PacketType.h"

class ChatHandler
{
public:
	ChatHandler();
	~ChatHandler();

	static void HandleEvent( PacketHeader& pktBase );
};

