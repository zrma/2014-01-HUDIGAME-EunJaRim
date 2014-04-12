#pragma once
#include "ClientSession.h"

class GameOverHandler
{
public:
	GameOverHandler();
	~GameOverHandler();

	static void HandleEvent( ClientSession* session, PacketHeader& pktBase );
};

