#pragma once
#include "PacketType.h"

class LoginHandler
{
public:
	LoginHandler();
	~LoginHandler();

	static void HandleEvent( PacketHeader& pktBase );
};

