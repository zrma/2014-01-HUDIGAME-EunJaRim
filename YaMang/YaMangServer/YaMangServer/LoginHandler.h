#pragma once
#include "ClientSession.h"

class LoginHandler
{
public:
	LoginHandler();
	~LoginHandler();

	static void HandleEvent( ClientSession* session, PacketHeader& pktBase );
};

